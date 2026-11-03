#include "macro/to_str.h"

#include <assert.h>
#include <string.h>
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_to_str() {
    TEST_ASSERT_EQUAL_STRING("123", TO_STR(123));
    TEST_ASSERT_EQUAL_STRING("\"abc\"", TO_STR("abc"));
    #define MY_DEF 0xABCD
    TEST_ASSERT_EQUAL_STRING("0xABCD", TO_STR(MY_DEF));
    TEST_ASSERT_EQUAL_STRING("a->b", TO_STR(a->b));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_to_str);
    return UNITY_END();
}
