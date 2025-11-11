#include "../include/command_parser.h"
#include "../include/config.h"
#include "../include/database.h"
#include "../include/services/journal.h"
#include "../include/services/log.h"
#include "../include/utils/str_utils.h"
#include "../include/utils/print_util.h"

int main() {
  StudentRecord *root = NULL;
  char input[256];
  const char *db_filename = DB_FILE; // db filename

  init_log();     // init log
  init_journal(); // init state manager & load last change_id

  printMenu();

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
