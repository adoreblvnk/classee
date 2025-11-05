#include "../include/command_parser.h"
#include "../include/utils.h"

int main() {
  StudentRecord *head = NULL;
  char input[256];
  const char *db_filename = "data/P3_7-CMS.txt"; // db filename

  printf("Class Management System (CMS) Initialized.\n");
  printf("Type 'OPEN' to load the database or 'EXIT' to quit.\n");

  while (1) {
    printf("P3_7: ");
    if (!fgets(input, sizeof(input), stdin)) { break; } // handle eof

    // exit condition (rmb to add \n)
    if (util_strcasecmp(input, "EXIT\n") == 0) { break; }

    processCommand(&head, input, db_filename);
  }

  freeList(head); // free mem before exit
  printf("CMS: Shutting down. All data has been cleared from memory.\n");
  return 0;
}
