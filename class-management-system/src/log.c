#include "../include/log.h"
#include "../include/cms.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int current_change_id = 0; // NOTE: keep this global only within log.c!
static const char *LOG_FILE = "data/cms.log";
static const char *TEMP_LOG_FILE = "data/cms.log.tmp";
static const char *LOG_HEADER = "change_id,command,id,name,programme,mark,time,is_change\n"; // 8

// getter func for current_change_id
int getCurrentChangeId(void) { return current_change_id; }

// init logger, creates log file if not exists, sets current_change_id to max change_id
void init_logger() {
  FILE *file = fopen(LOG_FILE, "r");
  if (!file) {
    file = fopen(LOG_FILE, "w");
    if (file) {
      fprintf(file, "%s", LOG_HEADER); // write header
      fclose(file);
    }
    current_change_id = 0;
    return;
  }
  char line[256];
  fgets(line, sizeof(line), file); // skip header
  int max_id = 0;
  while (fgets(line, sizeof(line), file)) {
    int id = atoi(line); // atoi stops at the first non-digit (the comma)
    if (id > max_id) { max_id = id; }
  }
  current_change_id = max_id;
  fclose(file);
}

// log cmd to log file
void logCommand(const char *command, int id, const char *name, const char *programme, float mark) {
  FILE *file = fopen(LOG_FILE, "a");
  if (!file) {
    perror("Failed to open log file");
    return;
  }
  current_change_id++;
  long int timestamp = time(NULL);

  fprintf(file, "%d,%s,", current_change_id, command);

  // TODO: only print details for insert/update/delete
  int is_change = 0;
  if (util_strcasecmp(command, "insert") == 0) {
    fprintf(file, "%d,%s,%s,%.1f,", id, name, programme, mark);
    is_change = 1;
  } else {
    fprintf(file, ",,,,"); // empty placeholders for non-modifying cmds
  }
  fprintf(file, "%ld,%d\n", timestamp, is_change);
  fclose(file);
}

// display entire command log
void showLog() {
  FILE *file = fopen(LOG_FILE, "r");
  if (!file) {
    printf("CMS: Log file not found.\n");
    return;
  }

  char line[512];
  printf("%-10s %-15s %-8s %-20s %-25s %-5s %-20s %-s\n", "Change ID", "Command", "ID", "Name",
         "Programme", "Mark", "Timestamp", "Change");

  fgets(line, sizeof(line), file); // skip header
  while (fgets(line, sizeof(line), file)) {
    char *cols[8];
    int col_count = 0;
    char *line_ptr = line;

    while ((cols[col_count] = util_strsep(&line_ptr, ",\n")) != NULL && col_count < 8) {
      col_count++;
    }

    if (col_count < 8) { continue; }

    time_t raw_time = atol(cols[6]);
    struct tm *time_info = localtime(&raw_time);
    char formatted_time[50];
    strftime(formatted_time, sizeof(formatted_time), "%d %m %y %H:%M:%S", time_info);

    printf("%-10s %-15s %-8s %-20s %-25s %-5s %-20s %-s\n", cols[0], cols[1], cols[2], cols[3],
           cols[4], cols[5], formatted_time, (atoi(cols[7]) == 1 ? "Yes" : "No"));
  }

  fclose(file);
}

// restore db state from log to target change id & truncates log file
// O(k * log n) avg. k=target_change_id, n=records. reads k lines, each insert is O(log n)
// O(k * n) worst case if tree is unbalanced (eg if every new id is lower than root id)
void restoreState(StudentRecord **root, int target_change_id) {
  FILE *log_file = fopen(LOG_FILE, "r");
  if (!log_file) {
    printf("CMS: Log file not found. Cannot restore.\n");
    return;
  }
  FILE *temp_log_file = fopen(TEMP_LOG_FILE, "w");
  if (!temp_log_file) {
    printf("CMS: Could not create temp file for restore.\n");
    fclose(log_file);
    return;
  }

  freeTree(*root); // clear current bst
  *root = NULL;

  char line[512];
  // copy header to temp log
  if (fgets(line, sizeof(line), log_file)) { fputs(line, temp_log_file); }
  while (fgets(line, sizeof(line), log_file)) {
    // exit early (break) if we've passed our target change id
    if (atoi(line) > target_change_id) { break; }

    fputs(line, temp_log_file); // write back line to temp log

    // now, process the line to rebuild the in-memory state
    char temp_line[512];
    strcpy(temp_line, line); // strsep modifies the string, so make a copy
    char *cols[8];
    int col_count = 0;
    char *line_ptr = temp_line;
    while ((cols[col_count] = util_strsep(&line_ptr, ",\n")) != NULL && col_count < 8) {
      col_count++;
    }
    if (col_count < 8 || atoi(cols[7]) == 0) { continue; } // NOTE: skip early if not a change

    if (util_strcasecmp(cols[1], "insert") == 0) {
      insertRecord(root, atoi(cols[2]), cols[3], cols[4], atof(cols[5]));
    }
    // TODO: add logic for 'update' & 'delete' if they are logged as changes
  }

  fclose(log_file);
  fclose(temp_log_file);

  // replace the old log with the new, truncated one
  remove(LOG_FILE);
  rename(TEMP_LOG_FILE, LOG_FILE);

  current_change_id = target_change_id; // update global id to the restored point
  printf("CMS: Database has been restored to change #%d. Log file has been truncated.\n",
         target_change_id);
}
