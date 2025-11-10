#ifndef JOURNAL_H
#define JOURNAL_H

#include "../types.h"

// init journal, creates file if not exists
void init_journal();

// log a state-changing cmd to journal file
void log_journal_command(const char *command, int id, const char *name, const char *programme,
                         float mark);

// display the mutable command journal (resettable history)
void showJournal();

// reset db state from journal to target change id & truncates journal file
void resetState(StudentRecord **root, int target_change_id);

#endif // JOURNAL_H
