#include "../include/utils.h"
#include <ctype.h> // for tolower
#include <stdio.h>
#include <stdlib.h>

// free all mem used by linked list
void freeList(StudentRecord *head) {
  StudentRecord *current = head;
  while (current != NULL) {
    StudentRecord *next = current->next;
    free(current);
    current = next;
  }
}

// print single student record, formatted
void printRecord(const StudentRecord *record) {
  if (record) {
    printf("%-8d %-20s %-25s %-5.1f\n", record->id, record->name, record->programme, record->mark);
  }
}

// our version of POSIX strcasecmp (case-insensitive string comparison)
int util_strcasecmp(const char *s1, const char *s2) {
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;
  int result;

  if (p1 == p2) { return 0; }

  while ((result = tolower(*p1) - tolower(*p2)) == 0) {
    if (*p1++ == '\0') { break; }
    p2++;
  }
  return result;
}
