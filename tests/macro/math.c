#include "cutils/macro/math.h"

#include <assert.h>
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_math_mod() {
    int val = MATH_MOD(10, 3);
    TEST_ASSERT_EQUAL_INT(val, 1);
    val = MATH_MOD(2, 3);
    TEST_ASSERT_EQUAL_INT(val, 2);
    val = MATH_MOD(-2, 3);
    TEST_ASSERT_EQUAL_INT(val, 1);
    val = MATH_MOD(0, 3);
    TEST_ASSERT_EQUAL_INT(val, 0);
}

void test_max() {
    int val = MAX(10, 3);
    TEST_ASSERT_EQUAL_INT(val, 10);
    val = MAX(3, 10);
    TEST_ASSERT_EQUAL_INT(val, 10);
}

void test_min() {
    int val = MIN(10, 3);
    TEST_ASSERT_EQUAL_INT(val, 3);
    val = MIN(3, 10);
    TEST_ASSERT_EQUAL_INT(val, 3);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_math_mod);
    RUN_TEST(test_max);
    RUN_TEST(test_min);
    return UNITY_END();
}
