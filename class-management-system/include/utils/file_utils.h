#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../types.h"

// helper func for showLog and showJournal
void print_log_or_journal_entry(char *line, int is_log);

// generic file handlers
void init_data_file(const char *filename, const char *header);
void display_data_file(const char *filename, const char *title_for_error,
                       const char *formatted_header, int is_log_format);

#endif // FILE_UTILS_H
