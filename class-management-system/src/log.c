#include "../include/log.h"
#include "../include/utils.h" // for util_str*
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int current_change_id = 0;
static const char *LOG_FILE = "data/cms.log";
static const char *LOG_HEADER = "change_id,command,id,name,programme,mark,time,is_change\n";

// getter func for the current change_id
int getCurrentChangeId(void) { return current_change_id; }

// getter func for the current change_id; increments before returning
int getCurrentChangeIdAndIncr(void) { return ++current_change_id; }

// ensures log file exists with a header and initializes change_id from it
void init_log() {
  // ensures log file exists with a header
  FILE *log_file = fopen(LOG_FILE, "a");
  if (log_file) {
    if (ftell(log_file) == 0) { fprintf(log_file, "%s", LOG_HEADER); }
    fclose(log_file);
  }

  // read the log to find the true max change_id
  log_file = fopen(LOG_FILE, "r");
  if (!log_file) {
    current_change_id = 0;
    return;
  }
  char line[256];
  fgets(line, sizeof(line), log_file); // skip header
  int max_id = 0;
  while (fgets(line, sizeof(line), log_file)) {
    int id = atoi(line); // atoi stops at 1st non-digit (the comma)
    if (id > max_id) { max_id = id; }
  }
  current_change_id = max_id;
  fclose(log_file);
}

// log any cmd (only append)
void log_command(const char *command, int id, const char *name, const char *programme, float mark,
                 int is_change) {
  FILE *log_file = fopen(LOG_FILE, "a");
  if (log_file) {
    int change_id = getCurrentChangeIdAndIncr();
    long int timestamp = time(NULL);
    fprintf(log_file, "%d,%s,", change_id, command);
    if (is_change) {
      fprintf(log_file, "%d,%s,%s,%.1f,", id, name, programme, mark);
    } else {
      fprintf(log_file, ",,,,"); // empty placeholders for non-modifying cmds
    }
    fprintf(log_file, "%ld,%d\n", timestamp, is_change);
    fclose(log_file);
  }
}

// display the immutable log entries (8-columns)
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
    char formatted_time[50];
    strftime(formatted_time, sizeof(formatted_time), "%d %m %y %H:%M:%S", localtime(&raw_time));
    printf("%-10s %-15s %-8s %-20s %-25s %-5s %-20s %-s\n", cols[0], cols[1], cols[2], cols[3],
           cols[4], cols[5], formatted_time, (atoi(cols[7]) == 1 ? "Yes" : "No"));
  }
  fclose(file);
}
