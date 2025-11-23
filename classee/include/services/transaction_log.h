#ifndef TRANSACTION_LOG_H
#define TRANSACTION_LOG_H

#include "../types.h"

// init transaction log, creates file if not exists
void init_tlog();

// log a state-changing cmd to transaction log file
void log_transaction(const char *command, int id, const char *name, const char *programme,
                     float mark);

// display the mutable command transaction log (rollback capable)
void show_tlog();

// performs point-in-time recovery of database to target change id & truncates transaction log
void perform_rollback(StudentRecord **root, int target_change_id);

#endif
