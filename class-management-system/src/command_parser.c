#include "../include/command_parser.h"
#include "../include/log.h"
#include "../include/summary.h"
#include "../include/utils.h"
#include <string.h>

// parse cmds & their args
void processCommand(StudentRecord **root, char *input, const char *db_filename) {
  char *command = strtok(input, " \n");
  if (!command) { return; }
  char *args = strtok(NULL, "\n");

  if (util_strcasecmp(command, "OPEN") == 0) {
    openDatabase(root, db_filename);
    logCommand("open", 0, NULL, NULL, 0.0);
  } else if (util_strcasecmp(command, "SHOW") == 0) {
    if (args && util_strcasecmp(args, "ALL") == 0) {
      showAll(*root);
      logCommand("show all", 0, NULL, NULL, 0.0);
    } else if (util_strcasecmp(args, "SUMMARY") == 0) {
      getAll(*root);
      logCommand("show summary", 0, NULL, NULL, 0.0);
    } else if (util_strcasecmp(args, "LOG") == 0) {
      showLog(); // we don't log LOG cmd itself
    } else {
      printf("CMS: Invalid argument for SHOW command.\n");
    }
  } else if (util_strcasecmp(command, "SAVE") == 0) {
    saveDatabase(*root, db_filename);
    logCommand("save", 0, NULL, NULL, 0.0);
  } else if (util_strcasecmp(command, "RESTORE") == 0) {
    if (args) {
      int change_id = atoi(args);
      if (change_id < 0 || change_id > getCurrentChangeId()) {
        printf("CMS: Invalid change ID for RESTORE. Choose a value between 1 & %d.\n",
               getCurrentChangeId());
      } else {
        restoreState(root, change_id);
      }
    } else {
      printf("CMS: Missing change ID for RESTORE.\n");
    } // we can't restore a RESTORE cmd so we don't log it
  } else {
    printf("CMS: Unknown command \"%s\".\n", command);
  }
}
