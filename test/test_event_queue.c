#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "event_queue.h"

void test_is_empty(void ** state) {
    (void) state;
    fail_msg("FUBAR");
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_is_empty)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

