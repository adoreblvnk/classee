#include "../../include/services/journal.h"
#include "../../include/database.h"
#include "../../include/services/log.h"
#include "../../include/utils/file_utils.h"
#include "../../include/utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *JOURNAL_FILE = "data/cms.journal";
static const char *TEMP_JOURNAL_FILE = "data/cms.journal.tmp";
static const char *JOURNAL_HEADER = "change_id,command,id,name,programme,mark,time\n";

// init journal
void init_journal() { init_data_file(JOURNAL_FILE, JOURNAL_HEADER); }

// log a state-changing cmd to journal file
// NOTE: call this func after log_command() as it increments global change_id counter
void log_journal_command(const char *command, int id, const char *name, const char *programme,
                         float mark) {
  FILE *journal_file = fopen(JOURNAL_FILE, "a");
  if (journal_file) {
    int change_id = getCurrentChangeId();
    long int timestamp = time(NULL);
    fprintf(journal_file, "%d,%s,%d,%s,%s,%.1f,%ld\n", change_id, command, id, name, programme,
            mark, timestamp);
    fclose(journal_file);
  }
}

// display the mutable command journal (7-columns)
void showJournal() {
  char header_buffer[256];
  sprintf(header_buffer, "%-10s %-15s %-8s %-20s %-25s %-5s %-20s", "Change ID", "Command", "ID",
          "Name", "Programme", "Mark", "Timestamp");
  display_data_file(JOURNAL_FILE, "Journal", header_buffer, 0); // 0 for journal
}

// reset db state from journal to target change id & truncates journal file
void resetState(StudentRecord **root, int target_change_id) {
  FILE *journal_file = fopen(JOURNAL_FILE, "r");
  if (!journal_file) {
    printf("CMS: Journal file not found. Cannot reset.\n");
    return;
  }
  FILE *temp_journal_file = fopen(TEMP_JOURNAL_FILE, "w");
  if (!temp_journal_file) {
    fclose(journal_file);
    printf("CMS: Could not create temp file for reset.\n");
    return;
  }
  freeTree(*root); // clear current bst
  *root = NULL;
  char line[512];
  // copy header to temp log
  if (fgets(line, sizeof(line), journal_file)) { fputs(line, temp_journal_file); }

  while (fgets(line, sizeof(line), journal_file)) {
    // exit early (break) if we've passed our target change id
    if (atoi(line) > target_change_id) { break; }
    fputs(line, temp_journal_file); // write back line to temp log

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
  fclose(journal_file);
  fclose(temp_journal_file);
  // replace the old log with the new, truncated one
  remove(JOURNAL_FILE);
  rename(TEMP_JOURNAL_FILE, JOURNAL_FILE);

  // NOTE: DON'T F***ING RESET THE GLOBAL change_id COUNTER ELSE THERE'LL BE DUPLICATES
  printf("CMS: Database state has been reset to change #%d.\n", target_change_id);
}
