#define LOG_LVL LOG_LVL_TRACE
#define LOG_GET_CONTEXT_FUNC log_get_context_func
#define LOG_FLUSH_FUNC log_flush_func
#include "cutils/errors.h"
#undef abort_progarm_func
#define abort_progarm_func() test_abort()

#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "unity.h"

#define TEST_ASSERT_CONTAIN_SUBSTR(str, substr) \
    TEST_ASSERT_TRUE(strstr(str, substr))

#define TEST_LOG_BUFFER_LEN 1024
static char buf[TEST_LOG_BUFFER_LEN];
static log_entry_context ctx = { .buf = buf, .capacity = TEST_LOG_BUFFER_LEN };
log_entry_context* log_get_context_func(void) {
    ctx.size = 0;
    return &ctx;
}

void log_flush_func(log_entry_context *ctx) {
}

void log_clear_buf() {
    ctx.size = 0;
    memset(ctx.buf, 0, TEST_LOG_BUFFER_LEN);
}

bool test_abort_called = false;
void test_abort() {
    test_abort_called = true;
}

void setUp(void) {}
void tearDown(void) {}

void test_do_if(void) {
    bool flag = true;
    log_clear_buf();
    do_if(false, flag = false);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_TRUE(flag);

    flag = true;
    log_clear_buf();
    do_if(false, flag = false, "test_msg");
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_TRUE(flag);

    flag = true;
    log_clear_buf();
    do_if(true, flag = false);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_FALSE(flag);

    flag = true;
    log_clear_buf();
    do_if(true, flag = false, "test_msg");
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_FALSE(flag);
}

void test_log_if(void) {
    log_clear_buf();
    log_if(false);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    log_clear_buf();
    log_if(false, "test_msg");
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    log_clear_buf();
    log_if(true);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    log_clear_buf();
    log_if(true, "test_msg");
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void test_ret_val_do_if(void) {}
void test_ret_val_do(void) {}
void test_ret_err_do_if(void) {}
void test_ret_err_do(void) {}
void test_ret_do_if(void) {}
void test_ret_do(void) {}
void test_ret_val_if(void) {}
void test_ret_val(void) {}
void test_ret_err_if(void) {}
void test_ret_err(void) {}
void test_ret_if(void) {}
void test_ret_(void) {}

void test_break_do_if(void) {
    bool flag = false;
    bool loop_flag = false;
    do {
        log_clear_buf();
        break_do_if(false, flag = true);
        TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
        TEST_ASSERT_FALSE(flag);

        log_clear_buf();
        break_do_if(false, flag = true, "test_msg");
        TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
        TEST_ASSERT_FALSE(flag);
        loop_flag = true;
    } while (false);
    TEST_ASSERT_TRUE(loop_flag);

    log_clear_buf();
    flag = false;
    while (true) {
        break_do_if(true, flag = true);
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_TRUE(flag);

    log_clear_buf();
    flag = false;
    while (true) {
        break_do_if(true, flag = true, "test_msg");
        TEST_ASSERT_TRUE(true);
    }
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void test_break_do(void) {
    bool flag = false;
    log_clear_buf();
    flag = false;
    while (true) {
        break_do(flag = true);
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_TRUE(flag);

    log_clear_buf();
    flag = false;
    while (true) {
        break_do(flag = true, "test_msg");
        TEST_ASSERT_TRUE(true);
    }
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void test_break_if(void) {
    bool loop_flag = false;
    do {
        log_clear_buf();
        break_if(false);
        TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

        log_clear_buf();
        break_if(false, "test_msg");
        TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
        loop_flag = true;
    } while (false);
    TEST_ASSERT_TRUE(loop_flag);

    log_clear_buf();
    while (true) {
        break_if(true);
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    log_clear_buf();
    while (true) {
        break_if(true, "test_msg");
        TEST_ASSERT_TRUE(true);
    }
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void test_break_(void) {
    log_clear_buf();
    while (true) {
        break_();
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    log_clear_buf();
    while (true) {
        break_("test_msg");
        TEST_ASSERT_TRUE(true);
    }
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void test_continue_do_if(void) {
    bool flag = false;
    bool loop_flag = false;
    do {
        log_clear_buf();
        continue_do_if(false, flag = true);
        TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
        TEST_ASSERT_FALSE(flag);

        log_clear_buf();
        continue_do_if(false, flag = true, "test_msg");
        TEST_ASSERT_FALSE(flag);
        TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
        loop_flag = true;
    } while (false);
    TEST_ASSERT_TRUE(loop_flag);

    int i;
    for (i = 0; i < 2; i++) {
        log_clear_buf();
        flag = false;
        continue_do_if(true, flag = true);
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_EQUAL_INT(i, 2);

    for (i = 0; i < 2; i++) {
        log_clear_buf();
        flag = false;
        continue_do_if(true, flag = true, "test_msg");
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_EQUAL_INT(i, 2);
}

void test_continue_do(void) {
    bool flag = false;
    int i;
    for (i = 0; i < 2; i++) {
        log_clear_buf();
        flag = false;
        continue_do(flag = true);
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_EQUAL_INT(i, 2);

    for (i = 0; i < 2; i++) {
        log_clear_buf();
        flag = false;
        continue_do(flag = true, "test_msg");
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_EQUAL_INT(i, 2);
}

void test_continue_if(void) {
    bool loop_flag = false;
    do {
        log_clear_buf();
        continue_if(false);
        TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

        log_clear_buf();
        continue_if(false, "test_msg");
        TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
        loop_flag = true;
    } while (false);
    TEST_ASSERT_TRUE(loop_flag);

    int i;
    for (i = 0; i < 2; i++) {
        log_clear_buf();
        continue_if(true);
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_EQUAL_INT(i, 2);

    for (i = 0; i < 2; i++) {
        log_clear_buf();
        continue_if(true, "test_msg");
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_EQUAL_INT(i, 2);
}

void test_continue_(void) {
    int i;
    for (i = 0; i < 2; i++) {
        log_clear_buf();
        continue_();
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_EQUAL_INT(i, 2);

    for (i = 0; i < 2; i++) {
        log_clear_buf();
        continue_("test_msg");
        TEST_ASSERT_TRUE(false);
    }
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_EQUAL_INT(i, 2);
}

void test_abort_do_if(void) {
    bool flag = false;
    test_abort_called = false;
    abort_do_if(false, flag = true);
    TEST_ASSERT_FALSE(flag);
    TEST_ASSERT_FALSE(test_abort_called);

    flag = false;
    test_abort_called = false;
    abort_do_if(false, flag = true, "test_msg");
    TEST_ASSERT_FALSE(flag);
    TEST_ASSERT_FALSE(test_abort_called);

    flag = false;
    test_abort_called = false;
    abort_do_if(true, flag = true);
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_TRUE(test_abort_called);

    flag = false;
    test_abort_called = false;
    abort_do_if(true, flag = true, "test_msg");
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_TRUE(test_abort_called);
}

void test_abort_do(void) {}
void test_abort_if(void) {}
void test_abort_(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_do_if);
    RUN_TEST(test_log_if);
    RUN_TEST(test_ret_val_do_if);
    RUN_TEST(test_ret_val_do);
    RUN_TEST(test_ret_err_do_if);
    RUN_TEST(test_ret_err_do);
    RUN_TEST(test_ret_do_if);
    RUN_TEST(test_ret_do);
    RUN_TEST(test_ret_val_if);
    RUN_TEST(test_ret_val);
    RUN_TEST(test_ret_err_if);
    RUN_TEST(test_ret_err);
    RUN_TEST(test_ret_if);
    RUN_TEST(test_ret_);
    RUN_TEST(test_break_do_if);
    RUN_TEST(test_break_do);
    RUN_TEST(test_break_if);
    RUN_TEST(test_break_);
    RUN_TEST(test_continue_do_if);
    RUN_TEST(test_continue_do);
    RUN_TEST(test_continue_if);
    RUN_TEST(test_continue_);
    RUN_TEST(test_abort_do_if);
    RUN_TEST(test_abort_do);
    RUN_TEST(test_abort_if);
    RUN_TEST(test_abort_);
    return UNITY_END();
}
