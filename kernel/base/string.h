/*
 * string.h
 *
 *  Created on: 07.05.2014
 *      Author: fabian
 */

#ifndef STRING_H_
#define STRING_H_

#include <base/stddef.h>

void* memset(void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);
void* memmove(void* destination, const void* source, size_t num);

char* strcpy(char* destination, const char* source);
char* strcat(char* destination, const char* source);

int strcmp(const char* str1, const char* str2);
int strncmp(const char* s1, const char* s2, size_t n);
size_t strlen(const char* str);

#endif /* STRING_H_ */
