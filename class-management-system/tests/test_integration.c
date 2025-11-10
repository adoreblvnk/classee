#include "../include/command_parser.h"
#include "../include/config.h"
#include "../include/database.h"
#include "../include/services/journal.h"
#include "../include/services/log.h"
#include "unity/src/unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void setup(void) {
  remove(DB_FILE);
  remove(LOG_FILE);
  remove(JOURNAL_FILE);
  // create mock data
  FILE *f = fopen(DB_FILE, "w");
  fprintf(f, "Database Name: INF1002-CMS\nAuthors: Test\n\nTable Name: "
             "StudentRecords\nID\tName\tProgramme\tMark\n1\tAlice\tCS\t90.0\n");
  fclose(f);
  // only initialize once
  init_log();
  init_journal();
}

static void teardown(void) {
  remove(DB_FILE);
  remove(LOG_FILE);
  remove(JOURNAL_FILE);
}

// helper to read file into str
static char *read_file_content(const char *filename) {
  FILE *f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buffer = malloc(length + 1);
  if (buffer) {
    fread(buffer, 1, length, f);
    buffer[length] = '\0';
  }
  fclose(f);
  return buffer;
}

// open & save
void test_integration_open_save(void) {
  StudentRecord *root = NULL;

  processCommand(&root, "OPEN", DB_FILE);
  TEST_ASSERT_NOT_NULL(root);

  processCommand(&root, "SAVE", DB_FILE);
  char *content = read_file_content(DB_FILE);
  TEST_ASSERT_NOT_NULL(content);
  TEST_ASSERT_NOT_NULL(strstr(content, "1\tAlice\tCS\t90.0"));

  free(content);
  freeTree(root);
}

// test commands are logged to log & journal
void test_integration_cmd_is_logged(void) {
  log_command("insert", 3, "Charlie", "IS", 77.0, 1);
  log_journal_command("insert", 3, "Charlie", "IS", 77.0);

  char *log_content = read_file_content(LOG_FILE);
  TEST_ASSERT_NOT_NULL(log_content);

  // change_id is 1 cah setup() started count at 0.
  TEST_ASSERT_NOT_NULL(strstr(log_content, "1,insert,3,Charlie,IS,77.0"));
  free(log_content);
}

void run_integration_tests(void) {
  setup();
  RUN_TEST(test_integration_open_save);
  teardown();

  setup();
  RUN_TEST(test_integration_cmd_is_logged);
  teardown();
}
