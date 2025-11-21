#include "../include/command_parser.h"
#include "../include/services/log.h"
#include "../include/services/transaction_log.h"
#include "../include/summary.h"
#include "../include/utils/general_utils.h"
#include "../include/utils/print_util.h"
#include "../include/utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>

// parse cmds & their args using safer util_strsep
void processCommand(StudentRecord **root, char *input, const char *db_filename) {
  // NOTE: please use util_strsep & not strtok; strsep checks for NULL ptrs & empty strings
  char *command = util_strsep(&input, " \n");
  if (!command || *command == '\0') { return; }
  char *args = input;            // rest of str are args
  args[strcspn(args, "\n")] = 0; // rm \n
  // copy command to str for logging later
  char log_buf[256];                                          // copy full cmd after command
  snprintf(log_buf, sizeof(log_buf), "%s %s", command, args); // prevent sprintf overflow
  int log_flag = 0;

  if (util_strcasecmp(command, "OPEN") == 0) {
    openDatabase(root, db_filename);
  } else if (util_strcasecmp(command, "SHOW") == 0) {
    if (!args || *args == '\0') {
      printf("CMS: Missing argument for SHOW command (ALL, SUMMARY, LOG, JOURNAL).\n");
      return;
    }

    // parse SHOW arguments more carefully
    char *show_arg = util_strsep(&args, " \n");

    if (util_strcasecmp(show_arg, "ALL") == 0) {
      showAll(*root);
    } else if (util_strcasecmp(show_arg, "SUMMARY") == 0) {
      // show all if args is null, empty, or "ALL"
      if (!args || *args == '\0' || util_strcasecmp(args, "ALL") == 0) {
        displaysummary(*root, "ALL");
      } else {
        // otherwise, use args as the course filter
        displaysummary(*root, args);
      }
    } else if (util_strcasecmp(show_arg, "LOG") == 0) {
      showLog();
    } else if (util_strcasecmp(show_arg, "TLOG") == 0) {
      show_tlog();
    } else {
      printf("CMS: Invalid argument for SHOW command.\n");
      return;
    }
  } else if (util_strcasecmp(command, "SAVE") == 0) {
    saveDatabase(*root, db_filename);
  } else if (util_strcasecmp(command, "INSERT") == 0) {
    if (args) {
      PromptDataHolder data = stringTokenization(args);
      if (strlen(data.id) == 0 || strlen(data.mark) == 0 || strlen(data.programme) == 0 ||
          strlen(data.name) == 0) {
        printf("CMS: Missing arguments for INSERT. Use: INSERT ID=... Name=... Programme=... "
               "Mark=...\n");
        return;
      }

      if (!isValidStudentID(data.id)) {
        printf("Student ID type should be numbers only. Please retry.\n");
        return;
      }

      int id = atoi(data.id);
      if (studentExist(*root, id)) {
        printf("CMS: The record with ID=%d already exists\n", id);
        return;
      }

      if (!validLettersAndSpace(data.name) || !validLen(data.name)) {
        printf("Ensure name only consist of letters and spaces.\n");
        return;
      }

      if (!validLettersAndSpace(data.programme) || !validLen(data.programme)) {
        printf("Ensure programme name consist of letters and spaces\n");
        return;
      }

      if (!validFloat(data.mark)) {
        printf("Ensure proper float number or normal number.\n");
        return;
      }

      float mark = strtof(data.mark, NULL);
      insertRecord(root, id, data.name, data.programme, mark);
      printf("CMS: A new record with ID=%d is successfully inserted.\n", id);

      log_transaction("insert", id, data.name, data.programme, mark);
      log_flag = 1; // changes state
    }
  } else if (util_strcasecmp(command, "QUERY") == 0) {

    PromptDataHolder data = stringTokenization(args);
    if (!args || *args == '\0') {
      printf("CMS: Missing arguments for QUERY. Use: QUERY ID=...\n");
      return;
    }
    if (!isValidStudentID(data.id)) {
      printf("Student ID type should be numbers only. Please retry.\n");
      return;
    }
    queryStudent(*root, atoi(data.id));
  }

  else if (util_strcasecmp(command, "UPDATE") == 0) {
    if (args) {

      PromptDataHolder data = stringTokenization(args);
      if (!isValidStudentID(data.id)) {
        printf("Student ID type should be numbers only. Please retry.\n");
        return;
      }
     
      int id = atoi(data.id);
      StudentRecord *studentRecord = studentExist(*root, id);
      if (!studentRecord) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
        return;
      }

      if (strlen(data.name) > 0 && !validLettersAndSpace(data.name)) {
        printf("Ensure name consists of letters and spaces.\n");
        return;
      }

      if (!validLen(data.name)) strcpy(data.name, studentRecord->name);

      if (strlen(data.programme) > 0 && !validLettersAndSpace(data.programme)) {
        printf("Ensure programme name consists of letters and spaces\n");
        return;
      }

      if (!validLen(data.programme)) strcpy(data.programme, studentRecord->programme);

      float mark;

      if (!validLen(data.mark))
        mark = studentRecord->mark;
      else if (!validFloat(data.mark)) {
        printf("Ensure proper float number or real number.\n");
        return;
      } else
        mark = strtof(data.mark, NULL);

      updateRecord(studentRecord, data.name, data.programme, mark);
      printf("CMS: The record with ID=%d is successfully updated.\n", id);

      log_transaction("update", id, data.name, data.programme, mark);
      log_flag = 1; // changes state
    }
  }

  else if (util_strcasecmp(command, "DELETE") == 0) {
    if (args) {
      PromptDataHolder data = stringTokenization(args);

      if (!isValidStudentID(data.id)) {
        printf("Student ID type should be numbers only. Please retry.\n");
        return;
      }

      int id = atoi(data.id);
      if (!studentExist(*root, id)) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
        return;
      }

      char confirmationBuffer[6];
      int cmfBufferSize = sizeof(confirmationBuffer);

      printf("CMS: Are you sure you want to delete record with ID=%d? Type \"Y\" to confirm or "
             "type \"N\" to cancel.\nP3_7:",
             id);
      inputParser(confirmationBuffer, cmfBufferSize);

      if (util_strcasecmp("Y", confirmationBuffer) != 0) { // only proceed if input is "Y" or "y"
        printf("The deletion is cancelled.\n");
        return;
      }
      deleteRecord(root, id);
      if (util_strcasecmp("N", confirmationBuffer) == 0) {
        printf("The deletion is cancelled.\n");
        return;
      }

      deleteRecord(root, id);
      printf("CMS: The record with ID=%d is successfully deleted.\n", id);

      log_transaction("delete", id, "", "", 0.0);
      log_flag = 1; // changes state
    }
  }

  else if (util_strcasecmp(command, "HELP") == 0) {
    printMenu();
  } else if (util_strcasecmp(command, "ROLLBACK") == 0) {
    if (args && *args != '\0') {
      // validate that args is a positive number
      for (int i = 0; args[i] != '\0'; i++) {
        if (!isdigit(args[i])) {
          printf("CMS: Invalid change ID. Must be a positive number.\n");
          return; // exit early
        }
      }
      perform_rollback(root, atoi(args));
      log_flag = 1; // changes state
    } else {
      printf("CMS: Missing change ID for ROLLBACK.\n");
      return;
    }
  } else {
    printf("CMS: Unknown command \"%s\".\n", command);
    return;
  }

  log_command(log_buf, log_flag);
}
