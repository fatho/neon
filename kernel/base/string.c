/**
 * @file
 *
 * @author Fabian Thorand
 * @date   May 13, 2014
 *
 * @brief This module implements a few important string functions from the C standard.
 */
#include <base/string.h>

#include <base/stdint.h>
#include <base/stddef.h>

void* memset(void* ptr, int value, size_t num) {
    unsigned char* data = (unsigned char*) ptr;
    for (size_t i = 0; i < num; i++) {
        data[i] = (unsigned char) value;
    }
    return ptr;
}

void* memcpy(void* destination, const void* source, size_t num) {
    if(destination == source) {
        return destination;
    }
    char* dstptr = (char*) destination;
    char* srcptr = (char*) source;
    for(size_t i = 0; i < num; i++) {
        dstptr[i] = srcptr[i];
    }
    return destination;
}

void* memmove(void* destination, const void* source, size_t num) {
    if(source < destination) {
        char* dstptr = (char*) destination;
        char* srcptr = (char*) source;
        while(num--) {
            dstptr[num] = srcptr[num];
        }
        return destination;
    } else if (source > destination) {
        return memcpy(destination, source, num);
    } else {
        return destination;
    }
}

char* strcpy(char* destination, const char* source) {
    char* olddst = destination;
    while(*source) {
        *destination = *source;
        destination++;
        source++;
    }
    return olddst;
}

char* strcat(char* destination, const char* source) {
    char* dst = destination;
    while(*dst) dst++;
    strcpy(dst, source);
    return destination;
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2))
        str1++, str2++;
    return *(const unsigned char*) str1 - *(const unsigned char*) str2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n--)
        if (*s1++ != *s2++)
            return *(unsigned char*) (s1 - 1) - *(unsigned char*) (s2 - 1);
    return 0;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (*str++)
        len++;
    return len;
}
