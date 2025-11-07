#ifndef TYPES_H
#define TYPES_H

#define MAX_NAME_LEN 50
#define MAX_PROG_LEN 50

// student record BST
typedef struct StudentRecord {
  int id;
  char name[MAX_NAME_LEN];
  char programme[MAX_PROG_LEN];
  float mark;
  struct StudentRecord *left;  // for bst
  struct StudentRecord *right; // for bst
} StudentRecord;

#endif // TYPES_H
