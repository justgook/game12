/* Freestanding stub: stdlib.h */
#ifndef _STDLIB_H
#define _STDLIB_H

typedef unsigned long size_t;

extern void* malloc(size_t size);
extern void free(void* ptr);
extern void* calloc(size_t nmemb, size_t size);
extern void qsort(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*));

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif
