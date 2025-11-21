#ifndef DATABASE_H
#define DATABASE_H

#include "types.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// func prototypes

// core ops
void openDatabase(StudentRecord **root, const char *filename);
void showAll(const StudentRecord *root);
void insertRecord(StudentRecord **root, int id, const char *name, const char *programme,
                  float mark);
void saveDatabase(const StudentRecord *root, const char *filename);
void updateRecord(StudentRecord *root, const char *name, const char *programme, float mark);
void deleteRecord(StudentRecord **root, int id);
void queryStudent(const StudentRecord *root, int id);

// bst helpers
void freeTree(StudentRecord *root);

// db helper to find students
StudentRecord *studentExist(const StudentRecord *root, int id);

//sorting enums
typedef enum {
    SORT_BY_ID,
    SORT_BY_MARK
} SortField;

typedef enum {
    ORDER_ASC,
    ORDER_DESC
} SortOrder;

//Sorting Functions
StudentRecord *flattenTreeToArray(const StudentRecord *root, int *size);
void freeRecordArray(StudentRecord *arr);
void sortRecords(StudentRecord *arr, int size, SortField field, SortOrder order);
void printSortedRecords(const StudentRecord *arr, int size);

#endif // DATABASE_H
