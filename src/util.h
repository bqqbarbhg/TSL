#ifndef TSL_UTIL_H
#define TSL_UTIL_H

#ifdef _MSC_VER
#include <stdio.h>
#define TSL_SNPRINTF sprintf_s
#else
#include <stdio.h>
#define TSL_SNPRINTF snprintf
#endif

// Escapes non-drawable characters (includes 0-terminator)
// c: Buffer of at least 5 characters
void tsl_charEscape(char* c);

// Encodes an Unicode code point to UTF-8
// ptr: Pointer to at least 4 characters
// returns: One character past the last byte of the encoded character
//          `ptr` if `codepoint` is not a valid Unicode code point
char *tsl_pointToUtf8(char *ptr, unsigned int codepoint);

// Allocates a new chunk of memory and copies `size` bytes of `buf` to it
void *tsl_memdup(const void *buf, size_t size);

// Allocates a new string to heap and copies the contents of `str` to it
char *tsl_strdup(const char *str);

#endif
