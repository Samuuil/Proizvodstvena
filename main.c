#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "xor.h"
struct Card {
    char value;
    char color;
};

struct Player {
    vector_t *deck;
    int *cardAmounts;
    int num;
};

struct Card makeCard(char value, char color) {
    struct Card card;
    card.value = value;
    card.color = color;
    return card;
}

struct Player* makePerson(int num) {
    struct Player *player = (struct Player *)malloc(sizeof(struct Player));
    player->num = num;
    player->cardAmounts = (int *)calloc(14, sizeof(int));
    player->deck = init_vector();
    return player;
}

int getCardPoints(char value) {
    if (value == 'A') return 20;
    if (value == 'K') return 15;
    if (value == 'Q') return 12;
    if (value == 'J') return 10;
    return value - '0';
}

int getColorPoints(char color) {
    switch (color) {
        case 'S': return 1;
        case 'D': return 2;
        case 'H': return 3;
        case 'C': return 4;
    }
    return 0;
}

int getCardIndex(char value) {
    if (value >= '2' && value <= '9') return value - '2';
    switch (value) {
        case '1': return 9;
        case 'J': return 10;
        case 'Q': return 11;
        case 'K': return 12;
        case 'A': return 13;
    }
    return -1;
}

struct Card *readCard(FILE* fileptr) {
    char value, color;
    if (fscanf(fileptr, "%c %c ", &value, &color) != 2) {
        return NULL;
    }
    struct Card *card = (struct Card *)malloc(sizeof(struct Card));
    *card = makeCard(value, color);
    return card;
}

int compareCards(const void *a, const void *b) {
    struct Card *cardA = *(struct Card **)a;
    struct Card *cardB = *(struct Card **)b;

    int valueA = getCardPoints(cardA->value);
    int valueB = getCardPoints(cardB->value);

    if (valueA != valueB) {
        return valueB - valueA;
    }

    int colorA = getColorPoints(cardA->color);
    int colorB = getColorPoints(cardB->color);

    return colorB - colorA;
}

void sortDeck(vector_t *deck) {
    int size = getSize(deck);
    struct Card **cardsArray = (struct Card **)malloc(size * sizeof(struct Card *));
    for (int i = 0; i < size; i++) {
        cardsArray[i] = (struct Card *)getAt(deck, i);
    }

    qsort(cardsArray, size, sizeof(struct Card *), compareCards);

    for (int i = 0; i < size; i++) {
        setAt(deck, i, cardsArray[i]);
    }

    free(cardsArray);
}

void printResult(struct Player* player) {
    sortDeck(player->deck);
    printf("Player %d wins: ", player->num);
    for (int i = 0; i < getSize(player->deck); i++) {
        struct Card *card = (struct Card *)getAt(player->deck, i);
        printf("(%c, %c) ", card->value, card->color);
    }
    printf("\n");
}

void drawCards(char* filename, struct Player* players[4]) {
    FILE *fileptr = fopen(filename, "r");
    if (fileptr == NULL) {
        printf("File cannot be opened!!!\n");
        return;
    }

    int winners[4] = {0};
    int maxPlayer = -1, maxPoints = 0, flagForExiting = 0;

    struct Card *card = readCard(fileptr);
    while (card != NULL) {
        for (int i = 0; i < 4; i++) {
            if (card == NULL) break;
            push_back(players[i]->deck, card);
            players[i]->cardAmounts[getCardIndex(card->value)]++;
            if (players[i]->cardAmounts[getCardIndex(card->value)] == 4) {
                winners[i]++;
                flagForExiting++;
            }
            card = readCard(fileptr);
        }
        if (flagForExiting) break;
    }

    if (!flagForExiting) {
        for (int i = 0; i < 4; i++) {
            int playerPoints = 0;
            for (int j = 0; j < getSize(players[i]->deck); j++) {
                struct Card *card = (struct Card *)getAt(players[i]->deck, j);
                playerPoints += getCardPoints(card->value);
            }

            if (playerPoints > maxPoints) {
                maxPoints = playerPoints;
                maxPlayer = i;
            } else if (playerPoints == maxPoints) {
                int colorPoints1 = 0, colorPoints2 = 0;
                for (int j = 0; j < getSize(players[i]->deck); j++) {
                    struct Card *card1 = (struct Card *)getAt(players[i]->deck, j);
                    struct Card *card2 = (struct Card *)getAt(players[maxPlayer]->deck, j);
                    colorPoints1 += getColorPoints(card1->color);
                    colorPoints2 += getColorPoints(card2->color);
                }
                if (colorPoints1 > colorPoints2) {
                    maxPlayer = i;
                }
            }
        }
    } else {
        for (int i = 0; i < 4; i++) {
            if (winners[i]) {
                int playerPoints = 0;
                for (int j = 0; j < getSize(players[i]->deck); j++) {
                    struct Card *card = (struct Card *)getAt(players[i]->deck, j);
                    playerPoints += getCardPoints(card->value);
                }
                if (playerPoints > maxPoints) {
                    maxPoints = playerPoints;
                    maxPlayer = i;
                }
            }
        }
    }

    if (maxPlayer != -1) {
        printResult(players[maxPlayer]);
    } else {
        printf("No player meets the criteria.\n");
    }

    fclose(fileptr);
}

int main() {
    char* filename = "decrypted.txt";
    char* fileSizeName = "sizes.txt";
    // char mixedKey[100];
    struct Player* players[4];
    players[0] = makePerson(1);
    players[1] = makePerson(2);
    players[2] = makePerson(3);
    players[3] = makePerson(4);

    char keyString[100];
    getKey(keyString, "key.txt", 100);

    fileXOR("deck.txt", "encrypted.txt", keyString, "sizes.txt");
    fileXORWithSizes("encrypted.txt", "decrypted.txt", keyString, "sizes.txt");

    drawCards(filename, players);

    for (int i = 0; i < 4; i++) {
        clear(players[i]->deck);
        free(players[i]->cardAmounts);
        free(players[i]);
    }

    return 0;
}
