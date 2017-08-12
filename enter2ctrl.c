#include <stdio.h>
#include <stdlib.h>

// For strncasecmp + string.h
#include <strings.h>

#include <linux/input.h>

// NOTE: ##token pastes the token, #token "stringizes"
#define keycode(name) { .code = KEY_##name, .code_name = #name }

struct keycode_pair {
    unsigned int code;
    char * code_name;
};

const struct keycode_pair keycode_pairs[] =  {
    keycode(ESC),
    keycode(ENTER),
    keycode(RIGHTCTRL),
    keycode(LEFTCTRL),
    keycode(CAPSLOCK),

    // Other names
    {KEY_LEFTCTRL, "LCTRL"},
    {KEY_LEFTCTRL, "CTRL"},
    {KEY_RIGHTCTRL, "RCTRL"}
};

const unsigned int NUM_PAIRS = sizeof(keycode_pairs) / sizeof(struct keycode_pair);

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

const int MAX_NAME_SIZE = 100;

// Match a string to a KEY_CODE value
int find_key_code(char * key_str) {
    for(unsigned i = 0; i < NUM_PAIRS; i++) {
        struct keycode_pair kp = keycode_pairs[i];
        if(!strncasecmp(key_str, kp.code_name, MAX_NAME_SIZE)) {
            /* fprintf(stderr, "matched to keycode %s\n", kp.code_name); */
            return kp.code;
        }
    }
    return 0;
}

void set_key_code(char * key_str, struct input_event * event_up,
        struct input_event * event_down, struct input_event * event_repeat) {

    int code = find_key_code(key_str);
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

    set_key_code(argv[1], &main_key_up, &main_key_down, &main_key_repeat);
    set_key_code(argv[2], &hold_key_up, &hold_key_down, &hold_key_repeat);

    if(argc > 3) {
        set_key_code(argv[3], &tap_key_up, &tap_key_down, &tap_key_repeat);
    } else {
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
