#define LOG_LVL LOG_LVL_TRACE
#define LOG_GET_CONTEXT_FUNC log_get_context_func
#define LOG_FLUSH_FUNC log_flush_func
#include "cutils/errors.h"
#undef abort_program_func
#define abort_program_func() test_abort()

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

int test_counter;
int _ret_val_do_if() {
    test_counter = 0;
    ret_val_do_if(false, test_counter++, 10);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_val_do_if(true, test_counter++, 20);
    return 30;
}
int _ret_val_do_if_log() {
    test_counter = 0;
    ret_val_do_if(false, TEST_ASSERT_TRUE(false), 10, "test_msg");
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_val_do_if(true, test_counter = 2, 20, "test_msg");
    return 30;
}
void test_ret_val_do_if(void) {
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_val_do_if(), 20);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_EQUAL_INT(test_counter, 1);
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_val_do_if_log(), 20);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_EQUAL_INT(test_counter, 2);
}

int _ret_val_do() {
    test_counter = 0;
    ret_val_do(test_counter++, 20);
    return 30;
}
int _ret_val_do_log() {
    test_counter = 0;
    ret_val_do(test_counter = 2, 20, "test_msg");
    return 30;
}
void test_ret_val_do(void) {
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_val_do(), 20);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_EQUAL_INT(test_counter, 1);
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_val_do_log(), 20);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_EQUAL_INT(test_counter, 2);
}

int _ret_err_do_if() {
    test_counter = 0;
    ret_err_do_if(false, test_counter++);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_err_do_if(true, test_counter++);
    return 30;
}
int _ret_err_do_if_log() {
    test_counter = 0;
    ret_err_do_if(false, TEST_ASSERT_TRUE(false), "test_msg");
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_err_do_if(true, test_counter = 2, "test_msg");
    return 30;
}
void test_ret_err_do_if(void) {
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_err_do_if(), -1);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_EQUAL_INT(test_counter, 1);
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_err_do_if_log(), -1);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_EQUAL_INT(test_counter, 2);
}

int _ret_err_do() {
    test_counter = 0;
    ret_err_do(test_counter++);
    return 30;
}
int _ret_err_do_log() {
    test_counter = 0;
    ret_err_do(test_counter = 2, "test_msg");
    return 30;
}
void test_ret_err_do(void) {
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_err_do(), -1);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_EQUAL_INT(test_counter, 1);
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_err_do_log(), -1);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_EQUAL_INT(test_counter, 2);
}

void _ret_do_if() {
    test_counter = 0;
    ret_do_if(false, test_counter++);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_do_if(true, test_counter++);
    TEST_ASSERT_TRUE(false);
}
void _ret_do_if_log() {
    test_counter = 0;
    ret_do_if(false, TEST_ASSERT_TRUE(false), "test_msg");
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_do_if(true, test_counter = 2, "test_msg");
    TEST_ASSERT_TRUE(false);
}
void test_ret_do_if(void) {
    log_clear_buf();
    _ret_do_if();
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_EQUAL_INT(test_counter, 1);
    log_clear_buf();
    _ret_do_if_log();
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_EQUAL_INT(test_counter, 2);
}

void _ret_do() {
    test_counter = 0;
    ret_do(test_counter++);
    TEST_ASSERT_TRUE(false);
}
void _ret_do_log() {
    test_counter = 0;
    ret_do(test_counter = 2, "test_msg");
    TEST_ASSERT_TRUE(false);
}
void test_ret_do(void) {
    log_clear_buf();
    _ret_do();
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    TEST_ASSERT_EQUAL_INT(test_counter, 1);
    log_clear_buf();
    _ret_do_log();
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
    TEST_ASSERT_EQUAL_INT(test_counter, 2);
}

int _ret_val_if() {
    ret_val_if(false, 10);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_val_if(true, 20);
    return 30;
}
int _ret_val_if_log() {
    ret_val_if(false, 10, "test_msg");
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_val_if(true, 20, "test_msg");
    return 30;
}
void test_ret_val_if(void) {
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_val_if(), 20);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_val_if_log(), 20);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

int _ret_val() {
    ret_val(20);
    return 30;
}
int _ret_val_log() {
    ret_val(20, "test_msg");
    return 30;
}
void test_ret_val(void) {
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_val(), 20);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_val_log(), 20);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

int _ret_err_if() {
    ret_err_if(false);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_err_if(true);
    return 30;
}
int _ret_err_if_log() {
    ret_err_if(false, "test_msg");
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_err_if(true, "test_msg");
    return 30;
}
void test_ret_err_if(void) {
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_err_if(), -1);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_err_if_log(), -1);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

int _ret_err() {
    ret_err();
    return 30;
}
int _ret_err_log() {
    ret_err("test_msg");
    return 30;
}
void test_ret_err(void) {
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_err(), -1);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    log_clear_buf();
    TEST_ASSERT_EQUAL_INT(_ret_err_log(), -1);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void _ret_if() {
    ret_if(false);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_if(true);
    TEST_ASSERT_TRUE(false);
}
void _ret_if_log() {
    ret_if(false, "test_msg");
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    ret_if(true, "test_msg");
    TEST_ASSERT_TRUE(false);
}
void test_ret_if(void) {
    log_clear_buf();
    _ret_if();
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    log_clear_buf();
    _ret_if_log();
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void _ret_() {
    ret_();
    TEST_ASSERT_TRUE(false);
}
void _ret_log() {
    ret_("test_msg");
    TEST_ASSERT_TRUE(false);
}
void test_ret_(void) {
    log_clear_buf();
    _ret_();
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);
    log_clear_buf();
    _ret_log();
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

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
    log_clear_buf();
    abort_do_if(false, flag = true);
    TEST_ASSERT_FALSE(flag);
    TEST_ASSERT_FALSE(test_abort_called);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    flag = false;
    test_abort_called = false;
    log_clear_buf();
    abort_do_if(false, flag = true, "test_msg");
    TEST_ASSERT_FALSE(flag);
    TEST_ASSERT_FALSE(test_abort_called);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    flag = false;
    test_abort_called = false;
    log_clear_buf();
    abort_do_if(true, flag = true);
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_TRUE(test_abort_called);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    flag = false;
    test_abort_called = false;
    log_clear_buf();
    abort_do_if(true, flag = true, "test_msg");
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_TRUE(test_abort_called);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void test_abort_do(void) {
    bool flag = false;
    test_abort_called = false;
    log_clear_buf();
    abort_do(flag = true);
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_TRUE(test_abort_called);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    flag = false;
    test_abort_called = false;
    log_clear_buf();
    abort_do(flag = true, "test_msg");
    TEST_ASSERT_TRUE(flag);
    TEST_ASSERT_TRUE(test_abort_called);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void test_abort_if(void) {
    test_abort_called = false;
    log_clear_buf();
    abort_if(false);
    TEST_ASSERT_FALSE(test_abort_called);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    test_abort_called = false;
    log_clear_buf();
    abort_if(false, "test_msg");
    TEST_ASSERT_FALSE(test_abort_called);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    test_abort_called = false;
    log_clear_buf();
    abort_if(true);
    TEST_ASSERT_TRUE(test_abort_called);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    test_abort_called = false;
    log_clear_buf();
    abort_if(true, "test_msg");
    TEST_ASSERT_TRUE(test_abort_called);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

void test_abort_(void) {
    test_abort_called = false;
    log_clear_buf();
    abort_();
    TEST_ASSERT_TRUE(test_abort_called);
    TEST_ASSERT_EQUAL_size_t(ctx.size, 0);

    test_abort_called = false;
    log_clear_buf();
    abort_("test_msg");
    TEST_ASSERT_TRUE(test_abort_called);
    TEST_ASSERT_GREATER_OR_EQUAL_size_t(9, ctx.size);
    TEST_ASSERT_CONTAIN_SUBSTR(ctx.buf, "test_msg");
}

int test_compile_1(void) {
    int i = 0;
    do_if(true, i++);
    log_if(true);
    ret_val_do_if(true, 0, i++);
    ret_val_do(i++, 0);
    ret_err_do_if(true, i++);
    ret_err_do(i++);
    ret_val_if(true, 0);
    ret_val(0);
    ret_err_if(true);
    ret_err();
    while (0) {
        break_do_if(true, i++);
        break_do(i++);
        break_if(true);
        break_();
        continue_do_if(true, i++);
        continue_do(i++);
        continue_if(true);
        continue_();
    }
    abort_do_if(true, i++);
    abort_do(i++);
    abort_if(true);
    abort_();

    if (0)
        do_if(true, i++);
    else {
        do_if(true, i++);
    }
    if (0)
        log_if(true);
    else {
        log_if(true);
    }
    if (0)
        ret_val_do_if(true, 0, i++);
    else {
        ret_val_do_if(true, 0, i++);
    }
    if (0)
        ret_val_do(i++, 0);
    else {
        ret_val_do(i++, 0);
    }
    if (0)
        ret_err_do_if(true, i++);
    else {
        ret_err_do_if(true, i++);
    }
    if (0)
        ret_err_do(i++);
    else {
        ret_err_do(i++);
    }
    if (0)
        ret_val_if(true, 0);
    else {
        ret_val_if(true, 0);
    }
    if (0)
        ret_val(0);
    else {
        ret_val(0);
    }
    if (0)
        ret_err_if(true);
    else {
        ret_err_if(true);
    }
    if (0)
        ret_err();
    else {
        ret_err();
    }
    while (0) {
        if (0)
            break_do_if(true, i++)
        else {
            break_do_if(true, i++);
        }
        if (0)
            break_do(i++)
        else {
            break_do(i++);
        }
        if (0)
            break_if(true)
        else {
            break_if(true);
        }
        if (0)
            break_()
        else {
            break_();
        }
        if (0)
            continue_do_if(true, i++)
        else {
            continue_do_if(true, i++);
        }
        if (0)
            continue_do(i++)
        else {
            continue_do(i++);
        }
        if (0)
            continue_if(true)
        else {
            continue_if(true);
        }
        if (0)
            continue_()
        else {
            continue_();
        }
    }
    if (0)
        abort_do_if(true, i++);
    else {
        abort_do_if(true, i++);
    }
    if (0)
        abort_do(i++);
    else {
        abort_do(i++);
    }
    if (0)
        abort_if(true);
    else {
        abort_if(true);
    }
    if (0)
        abort_();
    else {
        abort_();
    }
}

void test_compile_2(void) {
    int i = 0;
    ret_do_if(true, i++);
    ret_do(i++);
    ret_if(true);
    ret_();
    if (0)
        ret_do_if(true, i++);
    else {
        ret_do_if(true, i++);
    }
    if (0)
        ret_do(i++);
    else {
        ret_do(i++);
    }
    if (0)
        ret_if(true);
    else {
        ret_if(true);
    }
    if (0)
        ret_();
    else {
        ret_();
    }
}

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
