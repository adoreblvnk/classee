#ifndef CMS_H
#define CMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h> // header for strcasecmp
#include "types.h"

// func prototypes

// core ops
void openDatabase(StudentRecord **head, const char *filename);
void showAll(const StudentRecord *head);
void saveDatabase(const StudentRecord *head, const char *filename);

#endif // CMS_H
