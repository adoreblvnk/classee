#include "../../include/services/transaction_log.h"
#include "../../include/config.h"
#include "../../include/database.h"
#include "../../include/services/log.h"
#include "../../include/utils/file_utils.h"
#include "../../include/utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *TLOG_HEADER = "change_id,command,id,name,programme,mark,time\n";

// init tlog
void init_tlog() { init_data_file(TLOG_FILE, TLOG_HEADER); }

// log a state-changing cmd to tlog file
// NOTE: call this func after log_command() as it increments global change_id counter
void log_transaction(const char *command, int id, const char *name, const char *programme,
                     float mark) {
  FILE *tlog_file = fopen(TLOG_FILE, "a");
  if (tlog_file) {
    int change_id = getCurrentChangeId();
    long int timestamp = time(NULL);
    fprintf(tlog_file, "%d,%s,%d,%s,%s,%.1f,%ld\n", change_id, command, id, name ? name : "",
            programme ? programme : "", mark, timestamp);
    fclose(tlog_file);
  }
}

// display the mutable command transaction log (7-columns)
void show_tlog() {
  char header_buffer[256];
  sprintf(header_buffer, "%-10s %-15s %-8s %-20s %-25s %-5s %-20s", "Change ID", "Command", "ID",
          "Name", "Programme", "Mark", "Timestamp");
  display_data_file(TLOG_FILE, "Tlog", header_buffer, 0); // 0 for tlog
}

// reset db state from tlog to target change id & truncates tlog file
void perform_rollback(StudentRecord **root, int target_change_id) {
  FILE *tlog_file = fopen(TLOG_FILE, "r");
  if (!tlog_file) {
    printf("CMS: Transaction log file not found. Cannot reset.\n");
    return;
  }
  FILE *temp_tlog_file = fopen(TLOG_TMP_FILE, "w");
  if (!temp_tlog_file) {
    fclose(tlog_file);
    printf("CMS: Could not create temp file for rollback.\n");
    return;
  }
  freeTree(*root); // clear current bst
  *root = NULL;
  char line[512];
  // copy header to temp log
  if (fgets(line, sizeof(line), tlog_file)) { fputs(line, temp_tlog_file); }

  while (fgets(line, sizeof(line), tlog_file)) {
    // exit early (break) if we've passed our target change id
    if (atoi(line) > target_change_id) { break; }
    fputs(line, temp_tlog_file); // write back line to temp log

    // now, process the line to rebuild the in-memory state
    char temp_line[512];
    strcpy(temp_line, line); // strsep modifies the string, so make a copy
    char *cols[7];
    int col_count = 0;
    char *line_ptr = temp_line;
    while ((cols[col_count] = util_strsep(&line_ptr, ",\n")) != NULL && col_count < 7) {
      col_count++;
    }
    if (col_count < 7) { continue; } // NOTE: skip early if not a change / malformed
    if (util_strcasecmp(cols[1], "insert") == 0) {
      insertRecord(root, atoi(cols[2]), cols[3], cols[4], atof(cols[5]));
    }
    // TODO: add logic for 'update' & 'delete'
  }
  fclose(tlog_file);
  fclose(temp_tlog_file);
  // replace the old log with the new, truncated one
  remove(TLOG_FILE);
  rename(TLOG_TMP_FILE, TLOG_FILE);

  // NOTE: DON'T F***ING RESET THE GLOBAL change_id COUNTER ELSE THERE'LL BE DUPLICATES
  printf("CMS: Database state has been reset to change #%d.\n", target_change_id);
}
