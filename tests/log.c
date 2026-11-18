#define LOG_LVL LOG_LVL_TRACE
#define LOG_GET_CONTEXT_FUNC log_get_context_func
#define LOG_FLUSH_FUNC log_flush_func
#include "cutils/log.h"

#include <assert.h>
#include <string.h>
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

#define TEST_LOG_BUFFER_LEN 1024
static char buf[TEST_LOG_BUFFER_LEN];
static log_entry_context ctx = { .buf = buf, .capacity = TEST_LOG_BUFFER_LEN };
log_entry_context* log_get_context_func(void) {
    ctx.size = 0;
    return &ctx;
}

void log_flush_func(log_entry_context *ctx) {
    puts(ctx->buf);
}

void test_log_entry(void) {
    char buf[TEST_LOG_BUFFER_LEN];
    log_entry_context ctx = {
        .buf = buf,
        .capacity = TEST_LOG_BUFFER_LEN,
        .size = 0
    };
    log_entry(&ctx, "test");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, TEST_LOG_BUFFER_LEN);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 5);
    TEST_ASSERT_EQUAL_STRING(buf, "test");

    ctx.size = 0;
    log_entry(&ctx, "val: %d, str: %s", 10, "str");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, TEST_LOG_BUFFER_LEN);
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

    ctx.size = 0;
    ctx.capacity = TEST_LOG_BUFFER_LEN;
    log_entry(&ctx, "too");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, TEST_LOG_BUFFER_LEN);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 4);
    TEST_ASSERT_EQUAL_STRING(buf, "too");
    log_entry(&ctx, "long");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, TEST_LOG_BUFFER_LEN);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 8);
    TEST_ASSERT_EQUAL_STRING(buf, "toolong");
    log_entry(&ctx, "log");
    TEST_ASSERT_EQUAL_PTR(ctx.buf, buf);
    TEST_ASSERT_EQUAL_size_t(ctx.capacity, TEST_LOG_BUFFER_LEN);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 11);
    TEST_ASSERT_EQUAL_STRING(buf, "toolonglog");
}

#define TEST_LOG_PREFIX(lvl, func, line)                                 \
    LOG_COLOR_ ##lvl "[" #lvl "]" LOG_COLOR_RESET " " LOG_COLOR_LOCATION \
    "tests/log.c:" #line " " func "():" LOG_COLOR_RESET

void test_log_lvl_funcs(void) {
    #define EXPECTED_LOG(lvl, line)                      \
        TEST_LOG_PREFIX(lvl, "test_log_lvl_funcs", line) \
        " Hello int: 1 float: 1.000000"
    log_trace("Hello", " int: ", 1, " float: ", 1.0);
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(TRACE, 88));
    log_debug("Hello", " int: ", 1, " float: ", 1.0);
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(DEBUG, 90));
    log_info("Hello", " int: ", 1, " float: ", 1.0);
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(INFO, 92));
    log_warn("Hello", " int: ", 1, " float: ", 1.0);
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(WARN, 94));
    log_error("Hello", " int: ", 1, " float: ", 1.0);
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(ERROR, 96));
    log_fatal("Hello", " int: ", 1, " float: ", 1.0);
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(FATAL, 98));
    #undef EXPECTED_LOG
}

static log_custom_log_entry(log_int_in_bin, int val);

static log_custom_log_entry(log_int_in_bin, int val) {
    int bit_cnt = 8;
    if (val) {
        bit_cnt = sizeof(val) * 8 - __builtin_clz(val);
        bit_cnt += 8 - bit_cnt % 8;
    }
    PriFmt("0b");
    while (bit_cnt--) {
        PriFmt("%c", (val & (1 << bit_cnt))? '1': '0');
    }
    return LOG_PRI_RET_VAL;
}

#define PriBit(val) PriCall(log_int_in_bin, val)

void test_log_custom_pri(void) {
    #define EXPECTED_LOG(line, msg) \
        TEST_LOG_PREFIX(DEBUG, "test_log_custom_pri", line) " " msg
    log_debug(PriBit(0));
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(123, "0b00000000"));
    log_debug(PriBit(7));
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(125, "0b00000111"));
    log_debug(PriBit(256));
    TEST_ASSERT_EQUAL_STRING(ctx.buf, EXPECTED_LOG(127, "0b0000000100000000"));
    #undef EXPECTED_LOG
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_log_entry);
    RUN_TEST(test_log_lvl_funcs);
    RUN_TEST(test_log_custom_pri);
    return UNITY_END();
}
