#include "../../include/services/log.h"
#include "../../include/config.h"
#include "../../include/utils/file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int current_change_id = 0;
static const char *LOG_HEADER = "change_id,command,time,is_change\n";

// getter func for the current change_id
int getCurrentChangeId(void) { return current_change_id; }

// getter func for the current change_id; increments before returning
int getCurrentChangeIdAndIncr(void) { return ++current_change_id; }

// ensures log file exists with a header and initializes change_id from it
void init_log() {
  init_data_file(LOG_FILE, LOG_HEADER);

  // read the log to find the true max change_id
  FILE *log_file = fopen(LOG_FILE, "r");
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
void log_command(const char *command, int is_change) {
  FILE *log_file = fopen(LOG_FILE, "a");
  if (log_file) {
    int change_id = getCurrentChangeIdAndIncr();
    long int timestamp = time(NULL);
    fprintf(log_file, "%d,%s,%ld,%d\n", change_id, command, timestamp, is_change);
    fclose(log_file);
  }
}

// display the immutable log entries (4-columns)
void showLog() {
  char header_buffer[256];
  sprintf(header_buffer, "%-10s %-80s %-18s %-s", "Change ID", "Command", "Timestamp", "Change");
  display_data_file(LOG_FILE, "Log", header_buffer, 1); // 1 for log
}
