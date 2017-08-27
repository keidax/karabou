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

void test_empty_queue_is_empty(void ** state) {
    event_queue_t *q = *state;

    input_event_t first_event = {0};
    input_event_t second_event = {0};
    input_event_t third_event = {0};

    add_to_queue(q, &first_event);
    add_to_queue(q, &second_event);
    add_to_queue(q, &third_event);

    pull_from_queue(q);
    assert_false(is_queue_empty(q));

    pull_from_queue(q);
    assert_false(is_queue_empty(q));

    pull_from_queue(q);
    assert_true(is_queue_empty(q));
    assert_null(q->front);
    assert_null(q->back);
}

void test_assert_on_empty(void ** state) {
    event_queue_t *q = *state;

    assert_true(is_queue_empty(q));

    q->front = (void *) 0x999;
    expect_assert_failure(is_queue_empty(q));

    q->back = (void *) 0x888;
    assert_false(is_queue_empty(q));

    q->front = NULL;
    expect_assert_failure(is_queue_empty(q));
}

void test_adds_to_back(void ** state) {
    event_queue_t * q = *state;

    input_event_t evt1 = {0};
    input_event_t evt2 = {0};

    assert_null(q->back);

    add_to_queue(q, &evt1);
    assert_ptr_equal(q->back->event, &evt1);
    assert_ptr_equal(q->front->event, &evt1);

    add_to_queue(q, &evt2);
    assert_ptr_equal(q->back->event, &evt2);
    assert_ptr_equal(q->front->event, &evt1);
}

void test_pulls_from_front(void ** state) {
    event_queue_t * q = *state;

    input_event_t evt1 = {0};
    input_event_t evt2 = {0};

    add_to_queue(q, &evt1);
    add_to_queue(q, &evt2);

    input_event_t * pulled_evt1 = pull_from_queue(q);
    assert_ptr_equal(pulled_evt1, &evt1);

    input_event_t * pulled_evt2 = pull_from_queue(q);
    assert_ptr_equal(pulled_evt2, &evt2);
}

void test_assert_on_pull(void ** state) {
    event_queue_t * q = *state;

    input_event_t evt = {0};
    add_to_queue(q, &evt);

    pull_from_queue(q);
    expect_assert_failure(pull_from_queue(q));
}

void test_next_order_is_correct(void ** state) {
    event_queue_t * q = *state;

    input_event_t evt1 = {0};
    input_event_t evt2 = {0};
    input_event_t evt3 = {0};

    add_to_queue(q, &evt1);
    add_to_queue(q, &evt2);
    add_to_queue(q, &evt3);

    assert_ptr_equal(q->front->event, &evt1);
    assert_ptr_equal(q->front->next->event, &evt2);
    assert_ptr_equal(q->front->next->next->event, &evt3);
}

void test_next_is_set_on_removal(void ** state) {
    event_queue_t * q = *state;

    input_event_t evt1 = {0};
    input_event_t evt2 = {0};
    input_event_t evt3 = {0};

    add_to_queue(q, &evt1);
    add_to_queue(q, &evt2);
    add_to_queue(q, &evt3);

    pull_from_queue(q);
    assert_ptr_equal(q->front->event, &evt2);
    assert_ptr_equal(q->front->next->event, &evt3);

    pull_from_queue(q);
    assert_ptr_equal(q->front->event, &evt3);
    assert_null(q->front->next);

    pull_from_queue(q);
    assert_null(q->front);
    assert_null(q->back);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_new_queue_is_empty, setup, teardown),
        cmocka_unit_test_setup_teardown(test_empty_queue_is_empty, setup, teardown),
        cmocka_unit_test_setup_teardown(test_assert_on_empty, setup, teardown),
        cmocka_unit_test_setup_teardown(test_adds_to_back, setup, teardown),
        cmocka_unit_test_setup_teardown(test_pulls_from_front, setup, teardown),
        cmocka_unit_test_setup_teardown(test_assert_on_pull, setup, teardown),
        cmocka_unit_test_setup_teardown(test_next_order_is_correct, setup, teardown),
        cmocka_unit_test_setup_teardown(test_next_is_set_on_removal, setup, teardown),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

