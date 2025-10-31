#include "../include/utils.h"
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