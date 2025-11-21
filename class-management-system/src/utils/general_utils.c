#include "../../include/utils/general_utils.h"
#include "../../include/types.h"
#include "../../include/utils/str_utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// student ID validator
bool isValidStudentID(const char *studentID) {
  if (strlen(studentID) != 7) {
    printf("CMS: Student ID length must be 7 digits. Please retry.\n");
    return false;
  }
  for (int i = 0; studentID[i] != '\0'; i++) {
    if (!isdigit(studentID[i])) {
      printf("CMS: Student ID must contain only numbers. Please retry.\n");
      return false;
    }
  }
  return true;
}

void inputParser(char *dataInputBuffer, int sizeOfDataInputBuffer) {
  if (!fgets(dataInputBuffer, sizeOfDataInputBuffer, stdin)) { return; }

  dataInputBuffer[strcspn(dataInputBuffer, "\n")] = 0; // rm \n
}

bool validLen(char *buffer) { return strlen(buffer) >= 1; }

bool validLettersAndSpace(char *buffer) {
  for (int i = 0; buffer[i] != '\0'; i++) {
    if (!isalpha(buffer[i]) && buffer[i] != ' ') return false;
  }
  return true;
}

bool validFloat(char *buffer) {
  char *p_bufferEnd;
  strtof(buffer, &p_bufferEnd);

  if (p_bufferEnd ==
      buffer) { // if endptr is at the start with buffer e.g. buffer = 'ab1' = 'a' , p_buffer = 'a'
    return false;
  }
  if (*p_bufferEnd != '\0') { // if at the end of the pointer does not equals to '\0', means invalid
                              // number too e.g. buffer = '12a' = 'a', p_buffer = 'a'
    return false;
  }
  return true;
}

static bool isKey(char *possibleKey) {
  char knownKey[4][10] = {"id", "programme", "mark", "name"};
  int keyLength = sizeof(knownKey) / sizeof(knownKey[0]);

  for (int i = 0; i < keyLength; i++) {
    if (util_strcasecmp(knownKey[i], possibleKey) == 0) return true;
  }

  return false;
}

// helper for stringTokenization only seen in this class
static void concatValueWithSpace(char *dest, size_t destSize, const char *value) {
  if (dest == NULL || destSize == 0 || value == NULL) return;

  size_t current_len = strlen(dest);

  if (current_len > 0) {
    // if [0] is not empty, we assume it contains a word and we go to the last index and add a
    // space.
    snprintf(dest + current_len, destSize - current_len, " %s", value);
  } else {
    snprintf(dest, destSize, "%s", value);
  }
}

static void applyKeyValue(PromptDataHolder *data, const char *key, const char *value) {
  if (!data || !key || !value) return;

  if (util_strcasecmp(key, "ID") == 0) {
    concatValueWithSpace(data->id, sizeof(data->id), value);
  } else if (util_strcasecmp(key, "PROGRAMME") == 0) {
    concatValueWithSpace(data->programme, sizeof(data->programme), value);
  } else if (util_strcasecmp(key, "MARK") == 0) {
    concatValueWithSpace(data->mark, sizeof(data->mark), value);
  } else if (util_strcasecmp(key, "NAME") == 0) {
    concatValueWithSpace(data->name, sizeof(data->name), value);
  }
}

PromptDataHolder stringTokenization(char *buffer) {
  PromptDataHolder data = {0};
  if (buffer == NULL) return data;

  char *sToken = strtok(buffer, " =");
  char currentKey[16] = ""; // initialize to empty string
  char currentValue[32];

  while (sToken != NULL) {
    if (isKey(sToken)) {
      strncpy(currentKey, sToken, sizeof(currentKey) - 1); // copy key safely
      currentKey[sizeof(currentKey) - 1] = '\0';           // ensure null-termination
    }

    sToken = strtok(NULL, " =");
    if (sToken == NULL) break;

    // copy value safely
    strncpy(currentValue, sToken, sizeof(currentValue) - 1);
    currentValue[sizeof(currentValue) - 1] = '\0';

    if (isKey(currentValue)) { continue; }

    // check key has been found before applying value
    if (currentKey[0] != '\0') { applyKeyValue(&data, currentKey, currentValue); }
  }

  return data;
}
