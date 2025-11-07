#include "../include/utils.h"
#include "unity/src/unity.h"

// setUp and tearDown are special functions Unity runs before and after each test
void setUp(void) {}

void tearDown(void) {}

void test_util_strcasecmp_return_zero_for_identical_strings(void) {
  TEST_MESSAGE("Testing util_strcasecmp with identical lowercase and uppercase strings.");
  TEST_ASSERT_EQUAL_INT(0, util_strcasecmp("hello", "hello"));
  TEST_ASSERT_EQUAL_INT(0, util_strcasecmp("WORLD", "WORLD"));
}

void test_util_strcasecmp_return_zero_for_different_case_strings(void) {
  TEST_MESSAGE("Testing util_strcasecmp with mixed-case strings that should be equal.");
  TEST_ASSERT_EQUAL_INT(0, util_strcasecmp("HeLLo", "hElLo"));
  TEST_ASSERT_EQUAL_INT(0, util_strcasecmp("TeStInG", "tEsTiNg"));
}

void test_util_strcasecmp_return_non_zero_for_different_strings(void) {
  TEST_MESSAGE("Testing util_strcasecmp with completely different strings.");
  TEST_ASSERT_NOT_EQUAL(0, util_strcasecmp("hello", "world"));
}

// this function is called by the test runner
void run_utils_tests(void) {
  RUN_TEST(test_util_strcasecmp_return_zero_for_identical_strings);
  RUN_TEST(test_util_strcasecmp_return_zero_for_different_case_strings);
  RUN_TEST(test_util_strcasecmp_return_non_zero_for_different_strings);
}
