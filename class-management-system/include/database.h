#ifndef DATABASE_H
#define DATABASE_H

#include "types.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// func prototypes

// core ops
void openDatabase(StudentRecord **root, const char *filename);
void showAll(const StudentRecord *root);
void insertRecord(StudentRecord **root, int id, const char *name, const char *programme, float mark);
void saveDatabase(const StudentRecord *root, const char *filename);

// crud ops
StudentRecord *queryRecord(const StudentRecord *root, int id);
void updateRecord(StudentRecord *root, int id, const char *name, const char *programme, float mark);
StudentRecord *deleteRecord(StudentRecord **root, int id);

// sorting
void showAllSorted(const StudentRecord *root, const char *sort_by, const char *sort_order);

// bst helpers
void freeTree(StudentRecord *root);

#endif // DATABASE_H
