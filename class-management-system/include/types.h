#ifndef TYPES_H
#define TYPES_H

#define MAX_NAME_LEN 50
#define MAX_PROG_LEN 50

#define MAX_ID_LEN 32
#define MAX_CHAR_LEN 32

// student record BST
typedef struct StudentRecord {
  int id;
  char name[MAX_NAME_LEN];
  char programme[MAX_PROG_LEN];
  float mark;
  struct StudentRecord *left;  // for bst
  struct StudentRecord *right; // for bst
} StudentRecord;

// for prompts by insertions, update, delete, query
typedef struct PromptDataHolder  {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char programme[MAX_PROG_LEN];
    char mark[MAX_CHAR_LEN];
} PromptDataHolder;

#endif // TYPES_H
