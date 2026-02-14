/* Freestanding stub: assert.h */
#ifndef _ASSERT_H
#define _ASSERT_H

extern void _sokol_assert_fail(const char* expr, const char* file, int line);

#define assert(expr) ((expr) ? ((void)0) : _sokol_assert_fail(#expr, __FILE__, __LINE__))

#endif
