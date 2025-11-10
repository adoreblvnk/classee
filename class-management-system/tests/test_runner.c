#include "unity/src/unity.h"

void run_str_utils_tests(void);

int main(void) {
    UNITY_BEGIN();

    // run tests
    run_str_utils_tests();

    return UNITY_END();
}
