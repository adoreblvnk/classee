#ifndef LOG_H
#define LOG_H

#include "types.h"

// getter func for current_change_id
int getCurrentChangeId(void);

// init logger, creates log file if not exists, sets current_change_id to max change_id
void init_logger();

// log cmd to log file
void logCommand(const char *command, int id, const char *name, const char *programme, float mark);

// display entire command log
void showLog();

// restore db state from log to target change id & truncates log file
void restoreState(StudentRecord **head, int target_change_id);

#endif // LOG_H
