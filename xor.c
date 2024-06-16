#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#define MAX_BLOCK_SIZE 16

void getKey(char* keyString, const char* keyName, int keySize) {
    FILE* key = fopen(keyName, "r");

    if (key == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    fgets(keyString, keySize, key);
    fclose(key);
}

void fileXOR(const char* readName, const char* writeName, const char* key, const char* sizeFileName) {
    char buffer[MAX_BLOCK_SIZE];
    int keyLength = strlen(key);
    int keyIndex = 0;
    int blockSize = 8 + (key[0] % (MAX_BLOCK_SIZE - 8));

    FILE* read = fopen(readName, "rb");
    FILE* write = fopen(writeName, "wb");
    FILE* sizeFile = fopen(sizeFileName, "wb");

    if(read == NULL || write == NULL || sizeFile == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int bytesRead;
    while ((bytesRead = fread(buffer, 1, blockSize, read)) > 0) {
        fwrite(&blockSize, sizeof(int), 1, sizeFile);
        for (int i = 0; i < bytesRead; i++)
            buffer[i] ^= key[i % keyLength];

        fwrite(buffer, 1, bytesRead, write);
        blockSize = 8 + (buffer[0] % (MAX_BLOCK_SIZE - 8));
    }

    fclose(read);
    fclose(write);
    fclose(sizeFile);
}

void fileXORWithSizes(const char* readName, const char* writeName, const char* key, const char* sizeFileName) {
    char buffer[MAX_BLOCK_SIZE];
    int keyLength = strlen(key);
    int keyIndex = 0;

    FILE* read = fopen(readName, "rb");
    FILE* write = fopen(writeName, "wb");
    FILE* sizeFile = fopen(sizeFileName, "rb");

    if(read == NULL || write == NULL || sizeFile == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int bytesRead, blockSize;
    while (fread(&blockSize, sizeof(int), 1, sizeFile) > 0) {
        bytesRead = fread(buffer, 1, blockSize, read);
        for (int i = 0; i < bytesRead; i++)
            buffer[i] ^= key[i % keyLength];

        fwrite(buffer, 1, bytesRead, write);
    }

    fclose(read);
    fclose(write);
    fclose(sizeFile);
}

// int main() {
//     char keyString[100];
//     char mixedKey[100];

//     getKey(keyString, "key.txt", 17);
//     int keyLength = strlen(keyString);
//     mixKey(keyString, mixedKey, keyLength);

//     fileXOR("deck.txt", "encrypted.txt", mixedKey, "sizes.txt");
//     mixKey(keyString, mixedKey, keyLength);
//     fileXORWithSizes("encrypted.txt", "decrypted.txt", mixedKey, "sizes.txt");

//     return 0;
// }