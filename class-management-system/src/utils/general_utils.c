#include "../../include/utils/general_utils.h"
#include "../../include/utils/str_utils.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/types.h"

bool validStudentIDLen(char *studentID) {
    return strlen(studentID) == 7;
} 

bool validStudentIDType(char *studentID) {
    for (int i = 0; studentID[i] != '\0'; i++) {
        if(isdigit(studentID[i]) == 0) {
            return false;
        }
    }
    return true;
}

void inputParser(char *dataInputBuffer, int sizeOfDataInputBuffer) {
    if(!fgets(dataInputBuffer, sizeOfDataInputBuffer, stdin)) {
        return;
    }

    dataInputBuffer[strcspn(dataInputBuffer, "\n")] = 0; // rm \n
}

bool validLen(char *buffer) {
    return strlen(buffer) >= 1;
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

static bool isKey(char *possibleKey) {
    char knownKey[4][10] = {"id", "programme", "mark", "name"};
    int keyLength = sizeof(knownKey) / sizeof(knownKey[0]);

    for (int i = 0; i < keyLength; i++) {
        if(util_strcasecmp(knownKey[i], possibleKey) == 0) return true;
    }

    return false;

}

// helper for stringTokenization only seen in this class
static void concatValueWithSpace(char *dest, size_t destSize, const char *value) {

    // if [0] is not empty, we assume it contains a word and we go to the last index and add a space.
    if (dest[0] != '\0') {
        if (strcat_s(dest, destSize, " ") != 0) {
            printf("failed to add space\n");
            return;
        }
    }

    if (strcat_s(dest, destSize, value) != 0) {
        printf("failed to concat value '%s'\n", value);
    }
}


static void applyKeyValue(PromptDataHolder *data, const char *key, const char *value) {
    if (!data || !key || !value) return;

    if (util_strcasecmp(key, "ID") == 0) {
        concatValueWithSpace(data->id, sizeof(data->id), value);
    }
    else if (util_strcasecmp(key, "PROGRAMME") == 0) {
        concatValueWithSpace(data->programme, sizeof(data->programme), value);
    }
    else if (util_strcasecmp(key, "MARK") == 0) {
        concatValueWithSpace(data->mark, sizeof(data->mark), value);
    }
    else if (util_strcasecmp(key, "NAME") == 0) {
        concatValueWithSpace(data->name, sizeof(data->name), value);
    }
}

PromptDataHolder stringTokenization(char *buffer) {
    char *sToken = strtok(buffer, " =");
    char currentKey[16];
    char currentValue[32];
    PromptDataHolder data = {0};

    while (sToken != NULL) {
        if(isKey(sToken)){
            strcpy_s(currentKey, sizeof(currentKey), sToken);
        }

        sToken = strtok(NULL, " =");
        if (sToken == NULL) break;
        strcpy_s(currentValue, sizeof(currentValue), sToken);

        if (isKey(currentValue)) {continue;} 

        applyKeyValue(&data, currentKey, currentValue);
    }

    return data;
        
}



