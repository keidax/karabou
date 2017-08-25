#include <stdbool.h>
#include <stdint.h>
#include <linux/input.h>

#ifndef KEYS_H
#define KEYS_H

typedef struct input_event input_event_t;
typedef __u16 keycode_t;

// Represents an arbitrary group of keys with optional modifiers.
// All modifiers must be pressed before keys.
#define MAX_KEYGROUP_KEYS 10
#define MAX_KEYGROUP_MODS 10
typedef struct key_group {
    size_t mod_count;
    keycode_t mods[MAX_KEYGROUP_MODS];
    /* bool active_mods[MAX_KEYGROUP_MODS]; */

    size_t key_count;
    keycode_t keys[MAX_KEYGROUP_KEYS];
    /* bool active_keys[MAX_KEYGROUP_KEYS]; */
} key_group_t;


// Represents a map of keycodes and their names
typedef struct keycode_listing {
    keycode_t code;
    char * name;
} keycode_listing;
// NOTE: ##token pastes the token, #token "stringizes"
#define new_listing(kc_name) { .code = KEY_##kc_name, .name = #kc_name }



extern const int MAX_KEYCODE_NAME_SIZE;
extern const unsigned int NUM_KEYCODE_LISTINGS;

// Get the keycode for a keycode name string
extern keycode_t get_keycode(char * keycode_str);
// Initialize a key group based on a string
extern void init_key_group(char * key_group_str, key_group_t *);

// Returns the index of the matching modifier/key in the group, or -1
extern int match_key_group_mod(input_event_t * evt, key_group_t * group);
extern int match_key_group_key(input_event_t * evt, key_group_t * group);

#define EVENT_UP 0
#define EVENT_DOWN 1
#define EVENT_REPEAT 2
extern int match_event_type(input_event_t * evt, int type);

#endif // KEYS_H
