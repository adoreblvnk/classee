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
  if (!buffer) return data;

  char *p = buffer;
  const char *keys[] = {"ID", "NAME", "PROGRAMME", "MARK"}; // define keys as separators
  int num_keys = sizeof(keys) / sizeof(keys[0]);

  while (*p) {
    // skip leading whitespace
    while (*p && isspace((unsigned char)*p))
      p++;
    if (!*p) break;

    char *key_end = strchr(p, '=');
    if (!key_end) { break; } // malformed, no '=' found

    char *key = p;
    *key_end = '\0'; // terminate key
    char *val_start = key_end + 1;
    char *key_trim_end = key_end - 1;
    // trim trailing spaces before null termination
    while (key_trim_end >= key && isspace((unsigned char)*key_trim_end)) {
      key_trim_end--;
    }
    *(key_trim_end + 1) = '\0';
    char *val_end = val_start + strlen(val_start); // assume value is rest of str

    // find start of next key to correctly terminate current value
    char *next_key_marker = NULL;
    for (int i = 0; i < num_keys; i++) {
      char *found = util_strcasestr(val_start, keys[i]);
      // check if found key is followed by '='
      if (found && *(found + strlen(keys[i])) == '=') {
        // if we find a closer marker, update it
        if (!next_key_marker || found < next_key_marker) { next_key_marker = found; }
      }
    }

    if (next_key_marker) {
      val_end = next_key_marker;
      // trim trailing spaces from value before null-terminating
      while (val_end > val_start && isspace((unsigned char)*(val_end - 1))) {
        val_end--;
      }
      *val_end = '\0';
    }

    p = val_end; // move main ptr to end of current value
    applyKeyValue(&data, key, val_start);
  }
  return data;
}
