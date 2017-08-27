#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "event_queue.h"

int setup(void ** state) {
    event_queue_t * q = malloc(sizeof(event_queue_t));
    if(q == NULL) {
        return -1;
    }

    q->front = NULL;
    q->back = NULL;
    *state = q;

    return 0;
}

int teardown(void ** state) {
    free(*state);

    return 0;
}

void test_new_queue_is_empty(void ** state) {
    event_queue_t *q = *state;

    assert_true(is_queue_empty(q));
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_new_queue_is_empty, setup, teardown)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

