#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "database.h"

// parse cmds & their args
void processCommand(StudentRecord **root, char *input, const char *db_filename);

#endif // COMMAND_PARSER_H
