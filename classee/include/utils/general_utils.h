#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include <stdbool.h>
#include "../types.h"


// input parser to handle eof
void inputParser(char *dataInputBuffer, int sizeOfDataInputBuffer);

// validators
bool validStudentID(const char *studentID);
bool validLettersAndSpace(char *buffer);
bool validFloat(char *buffer);
bool validLen(char *buffer);
bool validNameProgrammeField(const char *input, const char *fieldName, bool isUpdateMode);

// for arguments
PromptDataHolder stringTokenization(char *buffer);


#endif
