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

// generic display func for log & journal
void display_data_file(const char *filename, const char *title_for_error,
                       const char *formatted_header, int is_log_format) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("CMS: %s file not found.\n", title_for_error);
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

// helper for showLog & showJournal, prints a formatted entry from a csv line
void print_log_or_journal_entry(char *line, int is_log) {
  int num_cols = is_log ? 8 : 7;
  char *cols[8]; // max of 8 cols
  int col_count = 0;
  char *line_ptr = line;

  // parse csv line
  while ((cols[col_count] = util_strsep(&line_ptr, ",\n")) != NULL && col_count < num_cols) {
    col_count++;
  }

  if (col_count < num_cols) { return; } // skip malformed lines

  // format timestamp
  time_t raw_time = atol(cols[6]);
  char formatted_time[50];
  strftime(formatted_time, sizeof(formatted_time), "%d %m %y %H:%M:%S", localtime(&raw_time));

  if (is_log) {
    printf("%-10s %-15s %-8s %-20s %-25s %-5s %-20s %-s\n", cols[0], cols[1], cols[2], cols[3],
           cols[4], cols[5], formatted_time, (atoi(cols[7]) == 1 ? "Yes" : "No"));
  } else { // is journal
    printf("%-10s %-15s %-8s %-20s %-25s %-5s %-20s\n", cols[0], cols[1], cols[2], cols[3], cols[4],
           cols[5], formatted_time);
  }
}
