#pragma once
#include <stdbool.h>
#include <stddef.h>

int strcmp(const char* s1, const char* s2);
size_t strlen(const char *str);
int strncmp( const char * s1, const char * s2, size_t n);
bool prefix(const char *pre, const char *str);
unsigned long strtoul(const char *__restrict nptr, int base);
