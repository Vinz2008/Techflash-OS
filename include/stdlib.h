// Get all of the various types
#define __need_wchar_t
#define __need_size_t
#define __need_ptrdiff_t
#define __need_wint_t
#define __need_NULL
#include <stddef.h>
char* itoa(long value, char* result, int base);
char* utoa(unsigned long value, volatile char* result, int base);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
int atoi(const char *nptr);