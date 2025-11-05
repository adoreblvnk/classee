#include "../include/command_parser.h"
#include "../include/utils.h"
#include <string.h>

// parse cmds & their args
void processCommand(StudentRecord **head, char *input, const char *db_filename) {
  char *command, *args;
  command = strtok(input, " \n");
  if (!command) { return; }

  if (util_strcasecmp(command, "OPEN") == 0) {
    openDatabase(head, db_filename);
  } else if (util_strcasecmp(command, "SHOW") == 0) {
    args = strtok(NULL, "\n");
    if (args && util_strcasecmp(args, "ALL") == 0) { showAll(*head); }
  } else if (util_strcasecmp(command, "SAVE") == 0) {
    saveDatabase(*head, db_filename);
  } else if (util_strcasecmp(command, "SUMMARY") == 0) {
    getAll(*head);
  } else if (util_strcasecmp(command, "EXIT") == 0) {
    // terminate loop
  } else {
    printf("CMS: Unknown command \"%s\".\n", command);
  }
}
