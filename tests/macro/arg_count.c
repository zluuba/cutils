#include "macro/arg_count.h"

#include <assert.h>
#include <stdbool.h>
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_arg_count(void) {
    TEST_ASSERT_EQUAL_INT(0, ARG_COUNT());
    TEST_ASSERT_EQUAL_INT(1, ARG_COUNT(0));
    TEST_ASSERT_EQUAL_INT(2, ARG_COUNT(0, 0));
    TEST_ASSERT (ARG_COUNT_IS_0(true, false));
    TEST_ASSERT (ARG_COUNT_IS_0(false, true, 0));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_arg_count);
    return UNITY_END();
}
