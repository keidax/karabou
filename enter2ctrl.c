#include <stdio.h>
#include <stdlib.h>

// For strncasecmp + string.h
#include <strings.h>

#include <linux/input.h>

#include "keys.h"

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

// Structure of input_event:
// type:  EV_KEY for keypress
// code:  keycodes (defined in input_event_codes.h)
// value: 1 for press, 0 for release, 2 for autorepeat

// clang-format off
struct input_event
main_key_up     = {.type = EV_KEY, .code = 0, .value = 0},
hold_key_up     = {.type = EV_KEY, .code = 0, .value = 0},
tap_key_up      = {.type = EV_KEY, .code = 0, .value = 0},
main_key_down   = {.type = EV_KEY, .code = 0, .value = 1},
hold_key_down   = {.type = EV_KEY, .code = 0, .value = 1},
tap_key_down    = {.type = EV_KEY, .code = 0, .value = 1},
main_key_repeat = {.type = EV_KEY, .code = 0, .value = 2},
hold_key_repeat = {.type = EV_KEY, .code = 0, .value = 2},
tap_key_repeat  = {.type = EV_KEY, .code = 0, .value = 2};
// clang-format on

key_group target_group = { 0 },
          hold_group = { 0 },
          tap_group = { 0 };

void set_key_code(char * key_str, struct input_event * event_up,
        struct input_event * event_down, struct input_event * event_repeat) {

    int code = get_keycode(key_str);
    if(!code) {
        fprintf(stderr, "could not find keycode for %s\n", key_str);
        exit(EXIT_FAILURE);
    }

    event_up->code = code;
    event_down->code = code;
    event_repeat->code = code;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        exit(EXIT_FAILURE);
    }

    init_key_group(argv[1], &target_group);
    init_key_group(argv[2], &hold_group);

    set_key_code(argv[1], &main_key_up, &main_key_down, &main_key_repeat);
    set_key_code(argv[2], &hold_key_up, &hold_key_down, &hold_key_repeat);

    if(argc > 3) {
        init_key_group(argv[3], &tap_group);
        set_key_code(argv[3], &tap_key_up, &tap_key_down, &tap_key_repeat);
    } else {
        init_key_group(argv[1], &tap_group);
        set_key_code(argv[1], &tap_key_up, &tap_key_down, &tap_key_repeat);
    }

    int main_is_down = 0, hold_give_up = 0;
    struct input_event input;

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (read_event(&input)) {
        // Skip non-key events
        if (input.type != EV_KEY) {
            write_event(&input);
            continue;
        }

        if (main_is_down) {
            if (equal(&input, &main_key_down) ||
                equal(&input, &main_key_repeat))
                continue;

            if (equal(&input, &main_key_up)) {
                main_is_down = 0;
                if (hold_give_up) {
                    hold_give_up = 0;
                    write_event(&hold_key_up);
                    continue;
                }
                write_event(&tap_key_down);
                write_event(&tap_key_up);
                continue;
            }

            if (!hold_give_up && input.value) {
                hold_give_up = 1;
                write_event(&hold_key_down);
            }
        } else if (equal(&input, &main_key_down)) {
            main_is_down = 1;
            continue;
        }

        write_event(&input);
    }
}
