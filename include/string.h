#ifndef _STRING_H
#define _STRING_H

size_t strlen(const char *str);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);


char *strstr(const char *haystack, const char *needle);

#endif
