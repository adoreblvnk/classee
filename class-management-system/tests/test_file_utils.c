#include "../include/config.h"
#include "../include/utils/file_utils.h"
#include "unity/src/unity.h"
#include <stdio.h>

static void setup(void) {
  remove(LOG_FILE);
  FILE *log = fopen(LOG_FILE, "w");
  if (log) {
    fprintf(log, "header\n%s", "1,open,,,,1762587206,0\n");
    fclose(log);
  }
}

static void teardown(void) { remove(LOG_FILE); }

// tests that init_data_file does not overwrite existing files
void test_init_data_file_not_overwrite_exist(void) {
  const char *expected_content = "1,open,,,,1762587206,0\n";

  init_data_file(LOG_FILE, "new_header\n"); // does nothiing

  FILE *file = fopen(LOG_FILE, "r");
  TEST_ASSERT_NOT_NULL(file);
  char line[256];
  fgets(line, sizeof(line), file); // skip header
  fgets(line, sizeof(line), file); // read data, don't need to loop
  fclose(file);

  TEST_ASSERT_EQUAL_STRING(expected_content, line); // original content unchanged
}

void run_file_utils_tests(void) {
  setup();
  RUN_TEST(test_init_data_file_not_overwrite_exist);
  teardown();
}
