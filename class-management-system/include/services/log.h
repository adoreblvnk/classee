#ifndef LOG_H
#define LOG_H

// getter for current change_id
int getCurrentChangeId(void);

// getter for current change_id and increment
int getCurrentChangeIdAndIncr(void);

// init log, creates file if not exists, and sets current_change_id from the file
void init_log();

// log any cmd to the immutable log file
void log_command(const char *command, int id, const char *name, const char *programme, float mark,
                 int is_change);

// display the immutable log entries
void showLog();

#endif // LOG_H
