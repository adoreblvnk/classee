#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "cms.h"

// parse cmds & their args
void processCommand(StudentRecord **head, char *input, const char *db_filename);

#endif // COMMAND_PARSER_H
