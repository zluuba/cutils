#include "cutils/log.h"

#include <assert.h>
#include <string.h>
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_log_entry() {
    char buf[1024];
    log_entry_context ctx = {
        .buf = buf,
        .capacity = 1024,
        .size = 0
    };
    log_entry(&ctx, "test");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, 1024);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 5);
    TEST_ASSERT_EQUAL_STRING(buf, "test");

    ctx.size = 0;
    log_entry(&ctx, "val: %d, str: %s", 10, "str");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, 1024);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 18);
    TEST_ASSERT_EQUAL_STRING(buf, "val: 10, str: str");

    ctx.size = 0;
    ctx.capacity = 5;
    log_entry(&ctx, "toolonglog");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, 5);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 5);
    TEST_ASSERT_EQUAL_STRING(buf, "tool");

    ctx.size = 0;
    ctx.capacity = 10;
    log_entry(&ctx, "toolonglog");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, 10);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 10);
    TEST_ASSERT_EQUAL_STRING(buf, "t...trunc");
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_log_entry);
    return UNITY_END();
}
