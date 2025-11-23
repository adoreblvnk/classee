#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <string.h> // for size_t

// our version of POSIX C string functions
int util_strcasecmp(const char *s1, const char *s2);
char *util_strcasestr(const char *haystack, const char *needle);
char *util_strsep(char **stringp, const char *delim);
void util_trim(char *str);

#endif // STR_UTILS_H
