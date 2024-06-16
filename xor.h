#ifndef XOR_H
#define XOR_H

void getKey(char* keyString, const char* keyName, int keySize);
void fileXOR(const char* readName, const char* writeName, const char* key, const char* sizeFileName);
void fileXORWithSizes(const char* readName, const char* writeName, const char* key, const char* sizeFileName);

#endif