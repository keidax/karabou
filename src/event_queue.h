#include <stdbool.h>

#include "keys.h"

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

// Represents one pending key event
typedef struct event_queue_entry {
    input_event_t * event;
    struct event_queue_entry * next;
} event_queue_entry_t;

// Contains a queue of pending key events
typedef struct event_queue {
    event_queue_entry_t * front;
    event_queue_entry_t * back;
} event_queue_t;

// Is the queue empty?
extern bool is_queue_empty(event_queue_t *);
// Add to the back of the queue
extern void add_to_queue(event_queue_t *, input_event_t *);
// Pull from the front of the queue
extern input_event_t * pull_from_queue(event_queue_t *);

#endif // EVENT_QUEUE_H
