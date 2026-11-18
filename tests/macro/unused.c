#include "cutils/macro/unused.h"
#include "cutils/macro/to_str.h"

#include <assert.h>
#include <string.h>
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_unused(void) {
    TEST_ASSERT_EQUAL_STRING(TO_STR(UNUSED(1, 2, 4)),
                             "(void)1; (void)2; (void)4;");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_unused);
    return UNITY_END();
}
