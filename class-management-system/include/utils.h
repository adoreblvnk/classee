#ifndef UTILS_H
#define UTILS_H

#include "types.h" // for StudentRecord

// helper funcs
void freeList(StudentRecord *head);
void printRecord(const StudentRecord *record);

#endif // UTILS_H
