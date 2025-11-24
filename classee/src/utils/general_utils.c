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
  for (int i = 0; studentID[i] != '\0'; i++) {
    if (!isdigit(studentID[i])) {
      printf("classee: Student ID must contain only numbers. Please retry.\n");
      return false;
    }
  }
  if (strlen(studentID) != 7) {
    printf("classee: Student ID length must be 7 digits. Please retry.\n");
    return false;
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
  float value = strtof(buffer, &p_bufferEnd);
  // if endptr is at the start with buffer e.g. buffer = 'ab1' = 'a' , p_buffer = 'a'
  if (p_bufferEnd == buffer) { return false; }
  // if at the end of the pointer does not equals to '\0', means invalid number too e.g. buffer =
  // '12a' = 'a', p_buffer = 'a'
  if (*p_bufferEnd != '\0') { return false; }
  // also check if float value is positive
  if (value < 0.0) { return false; }
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

static void applyKeyValue(PromptDataHolder *data, const char *key, const char *value) {
  if (!data || !key || !value) return;

  // trim leading/trailing whitespace from value before applying
  char trimmed_value[256];
  const char *start = value;
  while (*start && isspace((unsigned char)*start)) {
    start++;
  }
  const char *end = start + strlen(start) - 1;
  while (end > start && isspace((unsigned char)*end)) {
    end--;
  }
  strncpy(trimmed_value, start, end - start + 1);
  trimmed_value[end - start + 1] = '\0'; // rmb to null terminate

  if (util_strcasecmp(key, "ID") == 0) {
    strncpy(data->id, trimmed_value, sizeof(data->id) - 1);
    data->id[sizeof(data->id) - 1] = '\0';
  } else if (util_strcasecmp(key, "PROGRAMME") == 0) {
    strncpy(data->programme, trimmed_value, sizeof(data->programme) - 1);
    data->programme[sizeof(data->programme) - 1] = '\0';
  } else if (util_strcasecmp(key, "MARK") == 0) {
    strncpy(data->mark, trimmed_value, sizeof(data->mark) - 1);
    data->mark[sizeof(data->mark) - 1] = '\0';
  } else if (util_strcasecmp(key, "NAME") == 0) {
    strncpy(data->name, trimmed_value, sizeof(data->name) - 1);
    data->name[sizeof(data->name) - 1] = '\0';
  }
}

PromptDataHolder stringTokenization(char *buffer) {
  PromptDataHolder data = {0};
  if (!buffer) { return data; }

  char *p = buffer;
  const char *known_keys[] = {"ID", "NAME", "PROGRAMME", "MARK"};

  while (*p) {
    char *equals = strchr(p, '=');
    if (!equals) { break; }

    char *value_start = equals + 1;
    char *value_end = value_start + strlen(value_start); // assume value goes to end of str (\0)

    // find start of next key, which is the end of curr val
    for (int i = 0; i < 4; i++) {
      char *next_key = util_strcasestr(value_start, known_keys[i]);
      // if key is found & followed by '=' & is before current value_end
      // NOTE: value_end is updated to earliest next_key found
      if (next_key && strchr(next_key, '=') && next_key < value_end) { value_end = next_key; }
    }

    // terminate val for processing, value_start holds the val
    *equals = '\0';
    char end_char = *value_end;
    *value_end = '\0';
    char *key = p;
    util_trim(key);
    if (isKey(key)) {
      applyKeyValue(&data, key, value_start); // value_start is value
    } else {
      printf("classee: Invalid field '%s'. Use ID, NAME, PROGRAMME, or MARK.\n", key);
      *equals = '=';
      *value_end = end_char; // restores buffer that was changed to '\0'
      return (PromptDataHolder){0};
    }

    // restore buffer to original state
    *equals = '=';
    *value_end = end_char;
    p = value_end; // move p to value_end which is the start of next potential key value pair
  }
  return data;
}
