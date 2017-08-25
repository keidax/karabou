#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "event_queue.h"

bool is_queue_empty(event_queue_t * q) {
    // Both or neither should be null
    assert(q->front == NULL ? q->back == NULL : q->back != NULL);

    return q->front == NULL;
}

void add_to_queue(event_queue_t * q, input_event_t * evt) {
    event_queue_entry_t * new_entry = malloc(sizeof(event_queue_t));
    new_entry->event = evt;
    new_entry->next = NULL;

    if(is_queue_empty(q)) {
        q->front = new_entry;
    } else {
        q->back->next = new_entry;
    }
    q->back = new_entry;
}

input_event_t * pull_from_queue(event_queue_t * q) {
    // Can't pull from an empty queue
    assert(!is_queue_empty(q));

    event_queue_entry_t * front_entry = q->front;
    if(front_entry->next == NULL) {
        q->front = NULL;
        q->back = NULL;
    } else {
        q->front = front_entry->next;
    }

    input_event_t * evt = front_entry->event;
    free(front_entry);
    return evt;
}
