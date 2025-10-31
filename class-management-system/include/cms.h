#ifndef CMS_H
#define CMS_H

#include "types.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // header for strcasecmp

// func prototypes

// core ops
void openDatabase(StudentRecord **head, const char *filename);
void showAll(const StudentRecord *head);
void insertRecord(StudentRecord **head, int id, const char *name, const char *programme, float mark);
StudentRecord *queryRecord(const StudentRecord *head, int id);
void saveDatabase(const StudentRecord *head, const char *filename);

#endif // CMS_H
