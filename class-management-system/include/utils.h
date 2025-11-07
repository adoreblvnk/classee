#ifndef UTILS_H
#define UTILS_H

#include "types.h"  // for StudentRecord
#include <string.h> // for size_t

// helper funcs
void freeList(StudentRecord *head);
void printRecord(const StudentRecord *record);

// our version of POSIX C string functions
int util_strcasecmp(const char *s1, const char *s2);
char *util_strcasestr(const char *haystack, const char *needle);
char *util_strsep(char **stringp, const char *delim);

#endif // UTILS_H
