#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "bmp.h"

bool bmpCheckText(const char* text);
bool bmpCheckKey(const char* key);
char* strdup(const char* string);
char* vigenere_getNewText(const char* text);
char* vigenere_getNewKey(const char* key, const int length);

char* reverse(const char* text) {
    if (text == NULL) return NULL;

    char* reversed = strdup(text);

    for(int i = 0; i < strlen(text); i++) {
        reversed[strlen(text) - i - 1] = toupper(text[i]);
    }

    return reversed;
}

char* vigenere_encrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;
    if (bmpCheckKey(key) == false || bmpCheckText(text) == false) return NULL;

    int textLength = strlen(text);
    char* newKey = vigenere_getNewKey(key, textLength);
    char* newText = vigenere_getNewText(text);

    // encrypting
    char* encrypted = calloc(textLength + 1, sizeof(char));

    for(int i = 0, j = 0; i < textLength; i++) {
        if (isalpha(text[i]) == 0) {
            encrypted[i] = newText[i];
            continue;
        }

        encrypted[i] = ((newText[i] + newKey[j]) % 26) + 'A';
        j++;
    }

    encrypted[textLength] = '\0';

    free(newKey);
    free(newText);
    return encrypted;
}

char* vigenere_decrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;
    if (bmpCheckKey(key) == false || bmpCheckText(text) == false) return NULL;

    int textLength = strlen(text);
    char* newKey = vigenere_getNewKey(key, textLength);
    char* newText = vigenere_getNewText(text);

    // decrypting
    char* decrypted = calloc(textLength + 1, sizeof(char));

    for(int i = 0, j = 0; i < textLength; i++) {
        if (isalpha(text[i]) == 0) {
            decrypted[i] = newText[i];
            continue;
        }

        decrypted[i] = (((newText[i] - newKey[j]) + 26) % 26) + 'A';
        j++;
    }

    decrypted[textLength] = '\0';

    free(newKey);
    free(newText);
    return decrypted;
}

unsigned char* bit_encrypt(const char* text) {
    if (text == NULL) return NULL;
    if (bmpCheckText(text) == false) return NULL;

    int length = strlen(text);
    unsigned char* encrypted = calloc(length + 1, sizeof(char));

    for (int i = 0; i < length; i++){
        int firstFour = text[i] & 0xf0;
        int secondFour = text[i] & 0x0f;
        int firstResult = (((firstFour << 1) & 0xa0) | ((firstFour >> 1) & 0x50));
        int secondResult = (secondFour ^ (firstResult >> 4));
        encrypted[i] = (firstResult | secondResult);
    }

    encrypted[length] = '\0';
    return encrypted;
}

char* bit_decrypt(const unsigned char* text) {
    if (text == NULL) return NULL;
    if (bmpCheckText((char *) text) == false) return NULL;

    int length = strlen((char*) text);
    char* decrypted = calloc(length + 1, sizeof(char));

    for (int i = 0; i < strlen((char*) text); i++){
        int firstResult = text[i] & 0xf0;
        int secondResult = text[i] & 0x0f;
        int firstFour = (((firstResult << 1) & 0xa0) | ((firstResult >> 1) & 0x50));
        int secondFour = (secondResult ^ (firstResult >> 4));
        decrypted[i] = (firstFour | secondFour);
    }

    decrypted[length] = '\0';
    return decrypted;
}

unsigned char* bmp_encrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;
    if (bmpCheckKey(key) == false || bmpCheckText(text) == false) return NULL;

    char* reversed = reverse(text);
    char* vigenere = vigenere_encrypt(key, reversed);
    unsigned char* bit = bit_encrypt(vigenere);

    free(reversed);
    free(vigenere);
    return bit;
}

char* bmp_decrypt(const char* key, const unsigned char* text) {
    if (key == NULL || text == NULL) return NULL;
    if (bmpCheckKey(key) == false || bmpCheckText((char*) text) == false) return NULL;

    char* bit = bit_decrypt(text);
    char* vigenere = vigenere_decrypt(key, bit);
    char* reversed = reverse(vigenere);

    free(bit);
    free(vigenere);
    return reversed;
}

char* vigenere_getNewText(const char* text) {
    char* newText = strdup(text);

    for(int i = 0; i < strlen(text); i++) {
        newText[i] = toupper(text[i]);
    }

    return newText;
}

char* vigenere_getNewKey(const char* key, const int length) {
    char* newKey = calloc(length + 1, sizeof(char));

    for (int i = 0, j = strlen(key); i < length; i++, j++) {
        j %= strlen(key);
        newKey[i] = toupper(key[j]);
    }

    newKey[length] = '\0';
    return newKey;
}

bool bmpCheckKey(const char* key) {
    if (strlen(key) < 1) {
        return false;
    } else {
        for (int i = 0; i < strlen(key); i++) {
            if (isalpha(key[i]) == 0) {
                return false;
            }
        }
    }

    return true;
}

bool bmpCheckText(const char* text) {
    if (strlen(text) < 1) {
        return false;
    } else {
        int length = strlen(text);
        int spaces = 0;

        for (int i = 0; i < length; i++) {
            if (text[i] == ' ') {
                spaces++;
            }
        }

        if (spaces == length) {
            return false;
        }
    }

    return true;
}

char* strdup(const char* string) {
    char* duplicate = (char*) malloc((strlen(string) + 1) * sizeof(char));
    if (duplicate != NULL) strcpy(duplicate, string);
    return duplicate;
}
