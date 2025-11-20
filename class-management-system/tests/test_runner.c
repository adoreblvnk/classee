#include "unity/src/unity.h"

// setUp & tearDown is provided by unity
void setUp(void) {}
void tearDown(void) {}

// func prototypes for test suites
void run_str_utils_tests(void);
void run_file_utils_tests(void);

int main(void) {
  UNITY_BEGIN();

  // run all test suites
  run_str_utils_tests();
  run_file_utils_tests();

  return UNITY_END();
}
