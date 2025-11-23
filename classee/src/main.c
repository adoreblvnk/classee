#include "../include/command_parser.h"
#include "../include/config.h"
#include "../include/database.h"
#include "../include/services/log.h"
#include "../include/services/transaction_log.h"
#include "../include/utils/print_util.h"
#include "../include/utils/str_utils.h"

int main() {
  printDeclaration();
  StudentRecord *root = NULL;
  char input[256];
  const char *db_filename = DB_FILE; // db filename

  init_log();  // init log
  init_tlog(); // init state manager & load last change_id

  printMenu();

  while (1) {
    printf("P3_7: ");
    if (!fgets(input, sizeof(input), stdin)) { break; } // handle eof

    // clear stdin buffer if input was too long. This prevents overflow into next input
    // eg SHOW SUMMARY [very long course name] (> 255 chars)
    if (strchr(input, '\n') == NULL) { // check if newline is missing
      int c;
      while ((c = fgetc(stdin)) != '\n' && c != EOF) {} // discard extra chars
      printf("classee: Input exceeded 255 characters and was truncated.\n");
      continue;
    }

    // exit condition (rmb to add \n)
    // NOTE: handle exit here to exit loop
    if (util_strcasecmp(input, "EXIT\n") == 0) { break; }

    processCommand(&root, input, db_filename);
  }

  freeTree(root); // free mem before exit
  printf("classee: Shutting down. All data has been cleared from memory.\n");
  return 0;
}
