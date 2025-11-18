#include "../../include/utils/general_utils.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


bool validStudentIDLen(char *studentID) {
    if (strlen(studentID) == 7) return true;
    return false;
} 

bool validStudentIDType(char *studentID) {
    for (int i = 0; studentID[i] != '\0'; i++) {
        if(isdigit(studentID[i]) == 0) return false;
    }
    return true;
}

void inputParser(char *dataInputBuffer, int sizeOfDataInputBuffer) {
    if(!fgets(dataInputBuffer, sizeOfDataInputBuffer, stdin)) {
        return;
    }

    dataInputBuffer[strcspn(dataInputBuffer, "\n")] = 0; // rm \n
}

bool validLettersAndSpace(char *buffer) {
    for (int i = 0; buffer[i] != '\0'; i++){
        if(!isalpha(buffer[i]) && buffer[i] != ' ') return false;
    }
    return true;
}

bool validFloat(char *buffer) {
    char *p_bufferEnd;
    strtof(buffer, &p_bufferEnd);

    if (p_bufferEnd == buffer) { // if endptr is at the start with buffer e.g. buffer = 'ab1' = 'a' , p_buffer = 'a'
        return false; 
    }
    if (*p_bufferEnd != '\0') { // if at the end of the pointer does not equals to '\0', means invalid number too e.g. buffer = '12a' = 'a', p_buffer = 'a'
        return false;
    }
    return true;
    
}

