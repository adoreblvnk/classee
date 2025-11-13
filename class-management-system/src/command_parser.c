#include "../include/command_parser.h"
#include "../include/services/journal.h"
#include "../include/services/log.h"
#include "../include/summary.h"
#include "../include/utils/print_util.h"
#include "../include/utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// parse cmds & their args using safer util_strsep
void processCommand(StudentRecord **root, char *input, const char *db_filename) {
  // NOTE: please use util_strsep & not strtok; strsep checks for NULL ptrs & empty strings
  char *command = util_strsep(&input, " \n");
  if (!command || *command == '\0') { return; }
  char *args = input; // rest of str are args

  if (util_strcasecmp(command, "OPEN") == 0) {
    openDatabase(root, db_filename);
    log_command("open", 0, NULL, NULL, 0.0, 0);
  } else if (util_strcasecmp(command, "SHOW") == 0) {
    if (!args || *args == '\0') {
      printf("CMS: Missing argument for SHOW command (ALL, SUMMARY, LOG, JOURNAL).\n");
      return;
    }

    // parse SHOW arguments more carefully
    char *show_arg = util_strsep(&args, " \n");

    if (util_strcasecmp(show_arg, "ALL") == 0) {
      showAll(*root);
      log_command("show all", 0, NULL, NULL, 0.0, 0);
    } else if (util_strcasecmp(show_arg, "SUMMARY") == 0) {
      // show all if args is null, empty, or "ALL"
      if (!args || *args == '\0' || util_strcasecmp(args, "ALL") == 0) {
        displaysummary(*root, "ALL");
        log_command("show summary", 0, NULL, NULL, 0.0, 0);
      } else {
        // otherwise, use args as the course filter
        displaysummary(*root, args);
        log_command("show summary course", 0, args, NULL, 0.0, 0);
      }
    } else if (util_strcasecmp(show_arg, "LOG") == 0) {
      showLog();
      log_command("show log", 0, NULL, NULL, 0.0, 0);
    } else if (util_strcasecmp(show_arg, "JOURNAL") == 0) {
      showJournal();
      log_command("show journal", 0, NULL, NULL, 0.0, 0);
    } else {
      printf("CMS: Invalid argument for SHOW command.\n");
    }
  } else if (util_strcasecmp(command, "SAVE") == 0) {
    saveDatabase(*root, db_filename);
    log_command("save", 0, NULL, NULL, 0.0, 0);
  } else if (util_strcasecmp(command, "HELP") == 0) {
    printMenu();
    log_command("help", 0, NULL, NULL, 0.0, 0);
  } else if (util_strcasecmp(command, "RESET") == 0) {
    if (args && *args != '\0') {
      resetState(root, atoi(args));
      log_command("reset", atoi(args), NULL, NULL, 0.0, 0);
    } else {
      printf("CMS: Missing change ID for RESET.\n");
    }
  } else {
    printf("CMS: Unknown command \"%s\".\n", command);
  }
}
