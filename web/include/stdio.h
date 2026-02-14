/* Freestanding stub: stdio.h */
#ifndef _STDIO_H
#define _STDIO_H

#include "stdarg.h"
#include "stddef.h"

extern int snprintf(char* buf, size_t size, const char* fmt, ...);
extern int vsnprintf(char* buf, size_t size, const char* fmt, va_list ap);

#endif
