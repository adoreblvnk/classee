#include "../include/command_parser.h"
#include "../include/config.h"
#include "../include/database.h"
#include "../include/services/journal.h"
#include "../include/services/log.h"
#include "../include/utils/str_utils.h"

int main() {
  StudentRecord *root = NULL;
  char input[256];
  const char *db_filename = DB_FILE; // db filename

  init_log();     // init log
  init_journal(); // init state manager & load last change_id

  printf("Class Management System (CMS) Initialized.\n");
  printf("---------------------\n");
  printf("CMS Commands:\n");
  printf("  OPEN                - Open the database file.\n");
  printf("  SHOW ALL            - Display all student records.\n");
  printf("  SHOW SUMMARY ALL    - Display summary statistics for all courses.\n");
  printf("  SHOW SUMMARY [course_name] - Display summary statistics. Optionally filter by course.\n");
  printf("  SHOW LOG            - Display the command log.\n");
  printf("  SHOW JOURNAL        - Display the journal of changes.\n");
  printf("  SAVE                - Save the current database to file.\n");
  printf("  RESET <change_id>   - Reset the database to a previous state based on change ID.\n");
  printf("  HELP                - Show this help message.\n");
  printf("  EXIT                - Exit the program.\n");
  printf("\n");
  printf("---------------------\n");
  printf("Type open to load the database.\n");

  while (1) {
    printf("P3_7: ");
    if (!fgets(input, sizeof(input), stdin)) { break; } // handle eof

    // exit condition (rmb to add \n)
    // NOTE: handle exit here to exit loop
    if (util_strcasecmp(input, "EXIT\n") == 0) { break; }

    processCommand(&root, input, db_filename);
  }

  freeTree(root); // free mem before exit
  printf("CMS: Shutting down. All data has been cleared from memory.\n");
  return 0;
}
