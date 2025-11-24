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
      printf("classee: Missing argument for SHOW command (ALL, SUMMARY, LOG, JOURNAL).\n");
      return;
    }

    // parse SHOW arguments more carefully
    char *show_arg = util_strsep(&args, " \n");

    if (util_strcasecmp(show_arg, "ALL") == 0) {
      // If no extra arguments -> normal SHOW ALL
      if (!args || *args == '\0') {
        showAll(*root);
        return;
      }

      // Try to parse: SHOW ALL SORT BY MARK [ASC|DESC]
      char *arg1 = util_strsep(&args, " \n"); // SORT
      char *arg2 = util_strsep(&args, " \n"); // BY
      char *arg3 = util_strsep(&args, " \n"); // MARK
      char *arg4 = util_strsep(&args, " \n"); // optional ASC/DESC

      // We only handle MARK in this enhancement
      if (!(arg1 && util_strcasecmp(arg1, "SORT") == 0 && arg2 &&
            util_strcasecmp(arg2, "BY") == 0 && arg3 &&
            (util_strcasecmp(arg3, "MARK") == 0 || util_strcasecmp(arg3, "ID") == 0))) {
        printf("classee: Invalid SORT BY usage. Use: SHOW ALL SORT BY [MARK|ID] [ASC|DESC].\n");
        return;
      }

      SortOrder order = ORDER_ASC;
      if (arg4 && util_strcasecmp(arg4, "DESC") == 0) { order = ORDER_DESC; }

      int size = 0;
      StudentRecord *arr = flattenTreeToArray(*root, &size);

      if (!arr || size == 0) {
        printf("No records to display.\n");
        freeRecordArray(arr); // safe even if arr == NULL
        return;
      }

      SortField sort_field;
      if (util_strcasecmp(arg3, "MARK") == 0) {
        sort_field = SORT_BY_MARK;
      } else {
        sort_field = SORT_BY_ID;
      }

      sortRecords(arr, size, sort_field, order);
      printSortedRecords(arr, size);
      freeRecordArray(arr);
      return; // IMPORTANT: don't fall through
    }

    else if (util_strcasecmp(show_arg, "SUMMARY") == 0) {
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
      printf("classee: Invalid argument for SHOW command.\n");
      return;
    }
  } else if (util_strcasecmp(command, "SAVE") == 0) {
    saveDatabase(*root, db_filename);
  } else if (util_strcasecmp(command, "INSERT") == 0) {
    if (args) {
      PromptDataHolder data = stringTokenization(args);
      if (strlen(data.id) == 0 || strlen(data.mark) == 0 || strlen(data.programme) == 0 ||
          strlen(data.name) == 0) {
        printf("classee: Missing arguments for INSERT. Use: INSERT ID=... Name=... Programme=... "
               "Mark=...\n");
        return;
      }

      if (!isValidStudentID(data.id)) { return; }

      int id = atoi(data.id);
      if (studentExist(*root, id)) {
        printf("classee: The record with ID=%d already exists\n", id);
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
        printf("Ensure positive float number.\n");
        return;
      }

      float mark = strtof(data.mark, NULL);
      insertRecord(root, id, data.name, data.programme, mark);
      printf("classee: A new record with ID=%d is successfully inserted.\n", id);

      log_transaction("insert", id, data.name, data.programme, mark);
      log_flag = 1; // changes state
    }
  } else if (util_strcasecmp(command, "QUERY") == 0) {

    PromptDataHolder data = stringTokenization(args);
    if (!args || *args == '\0') {
      printf("classee: Missing arguments for QUERY. Use: QUERY ID=...\n");
      return;
    }
    if (!isValidStudentID(data.id)) { return; }
    queryStudent(*root, atoi(data.id));
  }

  else if (util_strcasecmp(command, "UPDATE") == 0) {
    if (args) {

      PromptDataHolder data = stringTokenization(args);
      if (!isValidStudentID(data.id)) { return; }

      int id = atoi(data.id);
      StudentRecord *studentRecord = studentExist(*root, id);
      if (!studentRecord) {
        printf("classee: The record with ID=%d does not exist.\n", id);
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
        printf("Ensure positive float number.\n");
        return;
      } else
        mark = strtof(data.mark, NULL);

      updateRecord(studentRecord, data.name, data.programme, mark);
      printf("classee: The record with ID=%d is successfully updated.\n", id);

      log_transaction("update", id, data.name, data.programme, mark);
      log_flag = 1; // changes state
    }
  }

  else if (util_strcasecmp(command, "DELETE") == 0) {
    if (args) {
      PromptDataHolder data = stringTokenization(args);

      if (!isValidStudentID(data.id)) { return; }

      int id = atoi(data.id);
      if (!studentExist(*root, id)) {
        printf("classee: The record with ID=%d does not exist.\n", id);
        return;
      }

      char confirmationBuffer[6];
      int cmfBufferSize = sizeof(confirmationBuffer);

      printf("classee: Are you sure you want to delete record with ID=%d? Type \"Y\" to confirm or "
             "type \"N\" to cancel.\nP3_7:",
             id);
      inputParser(confirmationBuffer, cmfBufferSize);
      util_trim(confirmationBuffer); // trim leading/trailing whitespace

      if (util_strcasecmp("Y", confirmationBuffer) == 0) {
        deleteRecord(root, id);
        printf("classee: The record with ID=%d is successfully deleted.\n", id);
        log_transaction("delete", id, "", "", 0.0);
        log_flag = 1; // changes state
      } else {
        printf("classee: The deletion is cancelled.\n");
      }
    }
  }

  else if (util_strcasecmp(command, "HELP") == 0) {
    printMenu();
  } else if (util_strcasecmp(command, "ROLLBACK") == 0) {
    if (args && *args != '\0') {
      // validate that args is a positive number
      for (int i = 0; args[i] != '\0'; i++) {
        if (!isdigit(args[i])) {
          printf("classee: Invalid change ID. Must be a positive number.\n");
          return; // exit early
        }
      }
      perform_rollback(root, atoi(args));
      log_flag = 1; // changes state
    } else {
      printf("classee: Missing change ID for ROLLBACK.\n");
      return;
    }
  } else {
    printf("classee: Unknown command \"%s\".\n", command);
    return;
  }

  log_command(log_buf, log_flag);
}
