#ifndef CMS_H
#define CMS_H

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

#endif // CMS_H
