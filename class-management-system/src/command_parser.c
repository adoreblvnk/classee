#include "../include/command_parser.h"
#include "../include/services/journal.h"
#include "../include/services/log.h"
#include "../include/summary.h"  
#include "../include/utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// parse cmds & their args
void processCommand(StudentRecord **root, char *input, const char *db_filename) {
  char *command = strtok(input, " \n");
  if (!command) { return; }
  char *args = strtok(NULL, "\n");
 

  if (util_strcasecmp(command, "OPEN") == 0) {
    openDatabase(root, db_filename);
    log_command("open", 0, NULL, NULL, 0.0, 0);
  } else if (util_strcasecmp(command, "SHOW") == 0) {
    if (!args) {
      printf("CMS: Missing argument for SHOW command (ALL, SUMMARY, LOG, JOURNAL).\n");
      return;
    }
    
    // Parse SHOW arguments more carefully
    char *show_arg = strtok(args, " \n");
    
    if (util_strcasecmp(show_arg, "ALL") == 0) {
      showAll(*root);
      log_command("show all", 0, NULL, NULL, 0.0, 0);
    } else if (util_strcasecmp(show_arg, "SUMMARY") == 0) {
      char *course_arg = args + strlen(show_arg) + 1;
      if (course_arg) {
        // SHOW SUMMARY <course_name> - use displaysummary with course name
        printf("Detected course argument for SUMMARY: %s\n", course_arg);
        displaysummary(*root, course_arg);
        log_command("show summary course", 0, course_arg, NULL, 0.0, 0);
        if (course_arg== "all" || course_arg== "ALL") {
          printf("Detected 'all' argument, displaying summary for all courses.\n");
          displaysummary(*root, "ALL");
          log_command("show summary", 0, NULL, NULL, 0.0, 0);
        }
      } 
      else {
        // SHOW SUMMARY - use displaysummary with "ALL"
        displaysummary(*root, "ALL");
        log_command("show summary", 0, NULL, NULL, 0.0, 0);
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
  } else if (util_strcasecmp(command, "RESET") == 0) {
    if (args) {
      resetState(root, atoi(args));
      log_command("reset", atoi(args), NULL, NULL, 0.0, 0);
    } else {
      printf("CMS: Missing change ID for RESET.\n");
    }
  } else {
    printf("CMS: Unknown command \"%s\".\n", command);
  }
}