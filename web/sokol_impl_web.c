/*
    sokol_impl_web.c

    Sokol implementation for wasm32-freestanding with GLES3/WebGL2 backend.
    Freestanding libc stubs + sokol_gfx/sokol_log implementation.
*/

/* ---- freestanding libc implementations ---- */

typedef unsigned long size_t;

/* WASM imports from host for memory allocation */
__attribute__((import_module("env")))
extern void* host_malloc(size_t size);

__attribute__((import_module("env")))
extern void host_free(void* ptr);

__attribute__((import_module("env")))
extern void host_log(const char* msg, int len);

/* memset */
void* memset(void* dest, int c, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    unsigned char val = (unsigned char)c;
    for (size_t i = 0; i < n; i++) d[i] = val;
    return dest;
}

/* memcpy */
void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (size_t i = 0; i < n; i++) d[i] = s[i];
    return dest;
}

/* memmove */
void* memmove(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    if (d < s) {
        for (size_t i = 0; i < n; i++) d[i] = s[i];
    } else {
        for (size_t i = n; i > 0; i--) d[i-1] = s[i-1];
    }
    return dest;
}

/* strlen */
size_t strlen(const char* s) {
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

/* strcmp */
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

/* strncmp */
int strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return (unsigned char)s1[i] - (unsigned char)s2[i];
        if (s1[i] == 0) return 0;
    }
    return 0;
}

/* malloc/free via host */
void* malloc(size_t size) { return host_malloc(size); }
void free(void* ptr) { host_free(ptr); }
void* calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void* ptr = host_malloc(total);
    if (ptr) memset(ptr, 0, total);
    return ptr;
}

/* minimal qsort (insertion sort) */
void qsort(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*)) {
    unsigned char* arr = (unsigned char*)base;
    unsigned char tmp[256];
    for (size_t i = 1; i < nmemb; i++) {
        size_t j = i;
        while (j > 0 && compar(arr + (j-1)*size, arr + j*size) > 0) {
            memcpy(tmp, arr + (j-1)*size, size);
            memcpy(arr + (j-1)*size, arr + j*size, size);
            memcpy(arr + j*size, tmp, size);
            j--;
        }
    }
}

/* assert handler */
void _sokol_assert_fail(const char* expr, const char* file, int line) {
    (void)file; (void)line;
    host_log(expr, strlen(expr));
    __builtin_trap();
}

/* minimal snprintf/vsnprintf */
int vsnprintf(char* buf, size_t size, const char* fmt, __builtin_va_list ap) {
    (void)ap;
    size_t flen = strlen(fmt);
    if (flen >= size) flen = size - 1;
    memcpy(buf, fmt, flen);
    buf[flen] = 0;
    return (int)flen;
}

int snprintf(char* buf, size_t size, const char* fmt, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    int ret = vsnprintf(buf, size, fmt, ap);
    __builtin_va_end(ap);
    return ret;
}

/* abort stub */
_Noreturn void abort(void) { __builtin_trap(); }

/* strncpy */
char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i]; i++) dest[i] = src[i];
    for (; i < n; i++) dest[i] = 0;
    return dest;
}

/* strstr */
char* strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;
    for (; *haystack; haystack++) {
        const char* h = haystack;
        const char* n = needle;
        while (*h && *n && *h == *n) { h++; n++; }
        if (!*n) return (char*)haystack;
    }
    return 0;
}

/* ---- Sokol implementation ---- */
#define SOKOL_IMPL
#define SOKOL_GLES3
#define SOKOL_EXTERNAL_GL_LOADER

#include "gl_funcs.h"
#include "../sokol_gfx.h"

/* Provide slog_func directly instead of using sokol_log.h (which has platform detection issues) */
void slog_func(const char* tag, unsigned int log_level, unsigned int log_item, const char* message, unsigned int line_nr, const char* filename, void* user_data) {
    (void)tag; (void)log_level; (void)log_item; (void)line_nr; (void)filename; (void)user_data;
    if (message) {
        host_log(message, strlen(message));
    }
}
