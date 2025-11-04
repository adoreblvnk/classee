#ifndef TYPES_H
#define TYPES_H

#define MAX_NAME_LEN 50
#define MAX_PROG_LEN 50

// student record struct
typedef struct StudentRecord {
  int id;
  char name[MAX_NAME_LEN];
  char programme[MAX_PROG_LEN];
  float mark;
  struct StudentRecord *next; // ts is a pointer to the next StudentRecord
} StudentRecord;

#endif // TYPES_H
