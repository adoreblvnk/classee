#include "../include/utils/str_utils.h"
#include "unity/src/unity.h"

void test_util_strcasecmp_mixed_case_str(void) {
  TEST_ASSERT_EQUAL_INT(0, util_strcasecmp("HeLLo", "hElLo"));
}

void test_util_strcasecmp_diff_str(void) {
  TEST_ASSERT_NOT_EQUAL(0, util_strcasecmp("hello", "world"));
}

void run_str_utils_tests(void) {
  RUN_TEST(test_util_strcasecmp_mixed_case_str);
  RUN_TEST(test_util_strcasecmp_diff_str);
}
