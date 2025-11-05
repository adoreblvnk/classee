#ifndef UTILS_H
#define UTILS_H

#include "types.h"  // for StudentRecord
#include <string.h> // for size_t

// helper funcs
void freeList(StudentRecord *head);
void printRecord(const StudentRecord *record);

int util_strcasecmp(const char *s1, const char *s2);

#endif // UTILS_H
