#include "../../include/utils/file_utils.h"
#include "../../include/utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ensures a data file exists with a header
void init_data_file(const char *filename, const char *header) {
  FILE *file = fopen(filename, "a");
  if (file) {
    if (ftell(file) == 0) { fprintf(file, "%s", header); }
    fclose(file);
  }
}

// generic display func for log & transaction log
void display_data_file(const char *filename, const char *title_for_error,
                       const char *formatted_header, int is_log_format) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("classee: %s file not found.\n", title_for_error);
    return;
  }
  char line[512];
  printf("%s\n", formatted_header);
  fgets(line, sizeof(line), file); // skip csv header

  while (fgets(line, sizeof(line), file)) {
    char temp_line[512];
    strcpy(temp_line, line);
    print_log_or_journal_entry(temp_line, is_log_format);
  }
  fclose(file);
}

// helper for showLog & show_tlog, prints a formatted entry from a csv line
void print_log_or_journal_entry(char *line, int is_log) {
  int num_cols_to_parse = is_log ? 4 : 7;
  char *cols[8]; // max of 8 cols is safe
  int col_count = 0;
  char *line_ptr = line;

  // parse csv line
  while ((cols[col_count] = util_strsep(&line_ptr, ",\n")) != NULL && col_count < num_cols_to_parse) {
    col_count++;
  }
  if (col_count < num_cols_to_parse) { return; } // skip malformed lines

  char formatted_time[50];
  const int time_col_idx = is_log ? 2 : 6; // time is col 2 for log, col 6 for tlog
  time_t raw_time = atol(cols[time_col_idx]);
  strftime(formatted_time, sizeof(formatted_time), "%d %m %y %H:%M:%S", localtime(&raw_time));
  if (is_log) {
    printf("%-10s %-80s %-18s %-s\n", cols[0], cols[1], formatted_time, (atoi(cols[3]) == 1 ? "Yes" : "No"));
  } else { // is transaction log
    printf("%-10s %-15s %-8s %-20s %-25s %-5s %-18s\n", cols[0], cols[1], cols[2], cols[3], cols[4], cols[5], formatted_time);
  }
}
