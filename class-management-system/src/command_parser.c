#include "../include/command_parser.h"
#include "../include/services/journal.h"
#include "../include/services/log.h"
#include "../include/summary.h"
#include "../include/utils/print_util.h"
#include "../include/utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>

// parse key=value pairs from a string, returns the value. modifies the input string.
static char *get_value_by_key(char *str, const char *key) {
  char *found = util_strcasestr(str, key);
  if (!found) return NULL;

  char *value_start = found + strlen(key);
  if (*value_start != '=') return NULL; // ensure it's key=value
  value_start++;                         // move past '='

  // find the start of the next potential key to correctly delimit values
  char *value_end = NULL;
  char *next_key_delimiters[] = {" ID=", " Name=", " Programme=", " Mark="};
  for (int i = 0; i < 4; i++) {
    char *next_key = util_strcasestr(value_start, next_key_delimiters[i]);
    if (next_key && (value_end == NULL || next_key < value_end)) { value_end = next_key; }
  }

  if (value_end) { *value_end = '\0'; }
  return value_start;
}

// parse cmds & their args using safer util_strsep
void processCommand(StudentRecord **root, char *input, const char *db_filename) {
  char *command = util_strsep(&input, " \n");
  if (!command || *command == '\0') { return; }
  char *args = input;
  if (args) { args[strcspn(args, "\n")] = 0; } // rm \n

  char log_buf[256];
  snprintf(log_buf, sizeof(log_buf), "%s %s", command, args ? args : "");
  int log_flag = 0;

  if (util_strcasecmp(command, "OPEN") == 0) {
    openDatabase(root, db_filename);
  } else if (util_strcasecmp(command, "SHOW") == 0) {
    if (!args || *args == '\0') {
      printf("CMS: Missing argument for SHOW command (ALL, SUMMARY, LOG, JOURNAL).\n");
      return;
    }

    char *show_arg = util_strsep(&args, " \n");

    if (util_strcasecmp(show_arg, "ALL") == 0) {
      if (args && util_strcasestr(args, "SORT BY")) {
        char *sort_by = util_strcasestr(args, "BY") + 3;
        char *sort_order = "ASC";
        char *desc_check = util_strcasestr(sort_by, "DESC");
        if (desc_check) {
          sort_order = "DESC";
          *desc_check = '\0';
        }
        char *sort_key = util_strsep(&sort_by, " \n");
        showAllSorted(*root, sort_key, sort_order);
      } else {
        showAll(*root);
      }
    } else if (util_strcasecmp(show_arg, "SUMMARY") == 0) {
      if (!args || *args == '\0' || util_strcasecmp(args, "ALL") == 0) {
        displaysummary(*root, "ALL");
      } else {
        displaysummary(*root, args);
      }
    } else if (util_strcasecmp(show_arg, "LOG") == 0) {
      showLog();
    } else if (util_strcasecmp(show_arg, "JOURNAL") == 0) {
      showJournal();
    } else {
      printf("CMS: Invalid argument for SHOW command.\n");
    }
  } else if (util_strcasecmp(command, "INSERT") == 0) {
    char args_copy[256];
    strncpy(args_copy, args, sizeof(args_copy) - 1);
    args_copy[sizeof(args_copy) - 1] = '\0';

    char *id_str = get_value_by_key(args_copy, "ID");
    char *name = get_value_by_key(args, "Name"); // Read from original args
    char *prog = get_value_by_key(args, "Programme");
    char *mark_str = get_value_by_key(args, "Mark");

    if (!id_str || !name || !prog || !mark_str) {
      printf("CMS: Missing arguments for INSERT. Use: INSERT ID=... Name=... Programme=... "
             "Mark=...\n");
    } else {
      int id = atoi(id_str);
      if (queryRecord(*root, id)) {
        printf("CMS: The record with ID=%d already exists.\n", id);
      } else {
        float mark = atof(mark_str);
        insertRecord(root, id, name, prog, mark);
        printf("CMS: A new record with ID=%d is successfully inserted.\n", id);
        log_flag = 1;
        log_journal_command("insert", id, name, prog, mark);
      }
    }
  } else if (util_strcasecmp(command, "QUERY") == 0) {
    char *id_str = get_value_by_key(args, "ID");
    if (!id_str) {
      printf("CMS: Missing ID for QUERY command. Use: QUERY ID=...\n");
    } else {
      int id = atoi(id_str);
      StudentRecord *rec = queryRecord(*root, id);
      if (rec) {
        printf("CMS: The record with ID=%d is found in the data table.\n", id);
        printf("%-8s %-20s %-25s %-5s\n", "ID", "Name", "Programme", "Mark");
        printf("%-8d %-20s %-25s %-5.1f\n", rec->id, rec->name, rec->programme, rec->mark);
      } else {
        printf("CMS: The record with ID=%d does not exist.\n", id);
      }
    }
  } else if (util_strcasecmp(command, "UPDATE") == 0) {
    char args_for_id[256];
    strncpy(args_for_id, args, sizeof(args_for_id) - 1);
    args_for_id[sizeof(args_for_id) - 1] = '\0';

    char *id_str = get_value_by_key(args_for_id, "ID");
    if (!id_str) {
      printf("CMS: Missing ID for UPDATE command. Use: UPDATE ID=... [Field=Value]\n");
      return;
    }
    int id = atoi(id_str);
    StudentRecord *record = queryRecord(*root, id);

    if (!record) {
      printf("CMS: The record with ID=%d does not exist.\n", id);
    } else {
      // Parse fields from the original, unmodified args string
      char *name = get_value_by_key(args, "Name");
      char *prog = get_value_by_key(args, "Programme");
      char *mark_str = get_value_by_key(args, "Mark");
      float mark = mark_str ? atof(mark_str) : -1.0f;

      // Apply the update to the in-memory record
      updateRecord(record, id, name, prog, mark);
      printf("CMS: The record with ID=%d is successfully updated.\n", id);
      log_flag = 1;

      // Log the full, new state of the record to the journal
      log_journal_command("update", record->id, record->name, record->programme, record->mark);
    }
  } else if (util_strcasecmp(command, "DELETE") == 0) {
    char *id_str = get_value_by_key(args, "ID");
    if (!id_str) {
      printf("CMS: Missing ID for DELETE command. Use: DELETE ID=...\n");
    } else {
      int id = atoi(id_str);
      if (!queryRecord(*root, id)) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
      } else {
        printf("CMS: Are you sure you want to delete record with ID=%d? Type \"Y\" to Confirm "
               "or type \"N\" to cancel.\n",
               id);
        char confirm[10];
        printf("P3_7: ");
        fgets(confirm, sizeof(confirm), stdin);
        if (util_strcasecmp(confirm, "Y\n") == 0) {
          *root = deleteRecord(root, id);
          printf("CMS: The record with ID=%d is successfully deleted.\n", id);
          log_flag = 1;
          log_journal_command("delete", id, "", "", 0.0);
        } else {
          printf("CMS: The deletion is cancelled.\n");
        }
      }
    }
  } else if (util_strcasecmp(command, "SAVE") == 0) {
    saveDatabase(*root, db_filename);
  } else if (util_strcasecmp(command, "HELP") == 0) {
    printMenu();
  } else if (util_strcasecmp(command, "RESET") == 0) {
    if (args && *args != '\0') {
      resetState(root, atoi(args));
      log_flag = 1;
    } else {
      printf("CMS: Missing change ID for RESET.\n");
    }
  } else {
    printf("CMS: Unknown command \"%s\".\n", command);
    return;
  }

  log_command(log_buf, log_flag);
}
