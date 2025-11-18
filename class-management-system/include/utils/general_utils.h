#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include <stdbool.h>

//helper func for checking studentID len
bool validStudentIDLen(char *studentID);

// helper func for checking student id data type
bool validStudentIDType(char *studentID);

// input parser to handle eof
void inputParser(char *dataInputBuffer, int sizeOfDataInputBuffer);

// validators
bool validLettersAndSpace(char *buffer);
bool validFloat(char *buffer);
bool validLen(char *buffer);




#endif