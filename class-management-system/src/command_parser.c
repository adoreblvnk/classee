#include "../include/command_parser.h"
#include "../include/services/transaction_log.h"
#include "../include/services/log.h"
#include "../include/summary.h"
#include "../include/utils/print_util.h"
#include "../include/utils/str_utils.h"
#include "../include/utils/general_utils.h"
#include <stdio.h>
#include <stdlib.h>

// parse cmds & their args using safer util_strsep
void processCommand(StudentRecord **root, char *input, const char *db_filename) {
  // NOTE: please use util_strsep & not strtok; strsep checks for NULL ptrs & empty strings
  char *command = util_strsep(&input, " \n");
  if (!command || *command == '\0') { return; }
  char *args = input; // rest of str are args
  args[strcspn(args, "\n")] = 0; // rm \n
  // copy command to str for logging later
  char log_buf[100]; // copy full cmd after command
  sprintf(log_buf, "%s %s", command, args);
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
  } 
    else if (util_strcasecmp(command, "INSERT") == 0) {
        if (args) {
            if(!validStudentIDType(args)){
                printf("Student ID type should be numbers only. Please retry.\n"); 
                return;
            } 
            if(!validStudentIDLen(args)){
                printf("Student ID len should be 7. Please retry.\n"); 
                return;
            } 
            int id = atoi(args);
            if (studentExist(*root, id)) {
                printf("CMS: The record with ID=%d already exists\n", id); 
                return;
            }

            char nameBuffer[256];
            char programmeBuffer[256];
            char markBuffer[256];
            //int id, const char *name, const char *programme, float mark
            printf("Adding new student record.\nPlease enter their name:\n");
            inputParser(nameBuffer, sizeof(nameBuffer));
            if (!validLettersAndSpace(nameBuffer) || !validLen(nameBuffer)) {
                printf("Ensure name only consist of letters and spaces.\n");
                return;
            }

            printf("Enter their programme:\n");
            inputParser(programmeBuffer, sizeof(programmeBuffer));
            if (!validLettersAndSpace(programmeBuffer) || !validLen(programmeBuffer)) {
                printf("Ensure programme name consist of letters and spaces\n");
                return;
            }

            printf("Enter their mark:\n");
            inputParser(markBuffer, sizeof(markBuffer));
            if (!validFloat(markBuffer)) {
                printf("Ensure proper float number or normal number.\n");
                return;
            }
            float mark = strtof(markBuffer, NULL);
            insertRecord(root, id, nameBuffer, programmeBuffer, mark);
            printf("CMS: A new record with ID=%d is successfully inserted.\n", id);
        }
    } 
    else if (util_strcasecmp(command, "QUERY") == 0) {
        if(args) {
            if(!validStudentIDType(args)){
                printf("Student ID type should be numbers only. Please retry.\n"); 
                return;
            } 
            if(!validStudentIDLen(args)){
                printf("Student ID len should be 7. Please retry.\n"); 
                return;
            } 
            queryStudent(*root, atoi(args));
        }
    }

    else if (util_strcasecmp(command, "UPDATE") == 0) {
        if(args) {
            if(!validStudentIDType(args)){
                printf("Student ID type should be numbers only. Please retry.\n"); 
                return;
            } 
            if(!validStudentIDLen(args)){
                printf("Student ID len should be 7. Please retry.\n"); 
                return;
            } 

            int id = atoi(args);
            StudentRecord *studentRecord = studentExist(*root, id);
            if (!studentRecord) {
                printf("CMS: The record with ID=%d does not exist.\n", id); 
                return;
            }

            char nameBuffer[256];
            char programmeBuffer[256];
            char markBuffer[256];

            printf("Updating student record for %d\n", id);
            printf("Usage: Press enter to skip current selector.\n");
            printf("New name: \n");
            inputParser(nameBuffer, sizeof(nameBuffer));
            
            if (!validLettersAndSpace(nameBuffer)) {
                printf("Ensure name consists of letters and spaces.\n");
                return;
            }

            if(!validLen(nameBuffer)) strcpy(nameBuffer,studentRecord->name);

            printf("Enter new programme:\n");
            inputParser(programmeBuffer, sizeof(programmeBuffer));
            if (!validLettersAndSpace(programmeBuffer)) {
                printf("Ensure programme name consists of letters and spaces\n");
                return;
            }

            if(!validLen(programmeBuffer)) strcpy(programmeBuffer, studentRecord->programme);

            float mark;
            printf("Enter new mark:\n");
            inputParser(markBuffer, sizeof(markBuffer));

            if(!validLen(markBuffer)) mark = studentRecord->mark;
            else if (!validFloat(markBuffer)) {
                printf("Ensure proper float number or real number.\n");
                return;
            }
            else mark = strtof(markBuffer, NULL);

            updateRecord(studentRecord, nameBuffer, programmeBuffer, mark);
            printf("CMS: The record with ID=%d is successfully updated.\n", id);
        }
    }

    else if (util_strcasecmp(command, "DELETE") == 0) {
        if(args) {
            if(!validStudentIDType(args)){
                printf("Student ID type should be numbers only. Please retry.\n"); 
                return;
            } 
            if(!validStudentIDLen(args)){
                printf("Student ID len should be 7. Please retry.\n"); 
                return;
            } 

            int id = atoi(args);
            if (!studentExist(*root, id)) {
                printf("CMS: The record with ID=%d does not exist.\n", id); 
                return;
            }

            deleteRecord(root, id);
            printf("CMS: The record with ID=%d is successfully deleted.\n", id);
        }
    }

    else if (util_strcasecmp(command, "HELP") == 0) {
    printMenu();
  } else if (util_strcasecmp(command, "ROLLBACK") == 0) {
    if (args && *args != '\0') {
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
