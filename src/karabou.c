#include <stdio.h>
#include <stdlib.h>

// For strncasecmp + string.h
#include <strings.h>

#include <linux/input.h>

#include "keys.h"
#include "event_queue.h"

int equal(const struct input_event *first, const struct input_event *second) {
    return first->type == second->type && first->code == second->code &&
           first->value == second->value;
}

int read_event(struct input_event *event) {
    return fread(event, sizeof(struct input_event), 1, stdin) == 1;
}

void write_event(const struct input_event *event) {
    if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
        exit(EXIT_FAILURE);
}

input_event_t * duplicate_event(input_event_t * evt) {
    input_event_t * new_evt = malloc(sizeof(input_event_t));

    new_evt->type = evt->type;
    new_evt->code = evt->code;
    new_evt->value = evt->value;

    return new_evt;
}

event_queue_t pending_queue = { NULL },
              active_queue = { NULL };

key_group_t target_group = {
        .mods = {},
        .mod_count = 0,
        .keys = { KEY_CAPSLOCK },
        .key_count = 1
    },
    replace_group = {
        .mods = {},
        .mod_count = 0,
        .keys = { KEY_ESC },
        .key_count = 1
    };

void release_queue(event_queue_t * q) {
    while(!is_queue_empty(q)) {
        input_event_t * evt = pull_from_queue(q);
        write_event(evt);
        free(evt);
    }
}

int main(int argc, char** argv) {
    struct input_event input;

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (read_event(&input)) {
        // Skip non-key events
        if (input.type != EV_KEY) {
            write_event(&input);
            continue;
        }

        // Duplicate event
        add_to_queue(&pending_queue, duplicate_event(&input));
        if (target_group.keys[0] == input.code) { // TODO: test against all targets
            // Traverse queue
            event_queue_entry_t * current_entry = pending_queue.front;

            while(current_entry) {
                if(current_entry->event->code == target_group.keys[0]) {
                    // TODO: perform full replacement
                    current_entry->event->code = replace_group.keys[0];
                    release_queue(&pending_queue);
                }
                current_entry = current_entry->next;
            }
        } else {
            release_queue(&pending_queue);
        }
    }
}
