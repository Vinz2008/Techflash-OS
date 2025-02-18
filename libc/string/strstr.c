#include <stdlib.h>
#include <string.h>

const char *strstr(const char *haystack, const char *needle) {
	size_t size = strlen(needle);

	while (*haystack) {
		if (!memcmp(haystack, needle, size)) {
			return haystack;
		}
		haystack++;
	}
	return NULL;
}