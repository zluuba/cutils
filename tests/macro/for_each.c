#include "cutils/macro/for_each.h"

#include <assert.h>
#include <stdbool.h>
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_for_each() {
    #define MY_ASSERT(arg) TEST_ASSERT(arg);
    FOR_EACH(MY_ASSERT);
    FOR_EACH(MY_ASSERT, true);
    FOR_EACH(MY_ASSERT, true, true);

    unsigned val = 0;
    #define SET_BIT(n) val |= 1 << n;

    val = 0;
    FOR_EACH(SET_BIT, 0, 1, 2, 3, 4, 5, 6, 7);
    TEST_ASSERT_EQUAL_INT(val, 0b11111111);

    val = 0;
    FOR_EACH(SET_BIT, 0, 2, 4, 6);
    TEST_ASSERT_EQUAL_INT(val, 0b01010101);

    val = 0;
    FOR_EACH(SET_BIT, 1, 3, 5, 7);
    TEST_ASSERT_EQUAL_INT(val, 0b10101010);
    return 0;
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_for_each);
    return UNITY_END();
}
