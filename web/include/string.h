/* Freestanding stub: string.h */
#ifndef _STRING_H
#define _STRING_H

typedef unsigned long size_t;

extern void* memset(void* dest, int c, size_t n);
extern void* memcpy(void* dest, const void* src, size_t n);
extern void* memmove(void* dest, const void* src, size_t n);
extern size_t strlen(const char* s);
extern int strcmp(const char* s1, const char* s2);
extern int strncmp(const char* s1, const char* s2, size_t n);

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif
