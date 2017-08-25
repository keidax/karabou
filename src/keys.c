#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include <linux/input.h>

#include "keys.h"

const keycode_listing keycode_listings[] =  {
    new_listing(ESC),
    new_listing(ENTER),
    new_listing(RIGHTCTRL),
    new_listing(LEFTCTRL),
    new_listing(CAPSLOCK),

    // Other names
    {KEY_LEFTCTRL, "LCTRL"},
    {KEY_LEFTCTRL, "CTRL"},
    {KEY_RIGHTCTRL, "RCTRL"}
};

const unsigned int NUM_KEYCODE_LISTINGS =
sizeof(keycode_listings) / sizeof(keycode_listing);

const int MAX_KEYCODE_NAME_SIZE = 100;

keycode_t get_keycode(char * keycode_str) {
    for(size_t i = 0; i < NUM_KEYCODE_LISTINGS; i++) {
        keycode_listing l= keycode_listings[i];
        if(!strncasecmp(keycode_str, l.name, MAX_KEYCODE_NAME_SIZE)) {
            return l.code;
        }
    }
    return 0;
}

const size_t MAX_MODS = 10;
const size_t MAX_KEYS = 10;

void init_key_group(char * key_group_str, key_group_t * new_key_group) {
    fprintf(stderr, "initializing key group: %s\n", key_group_str);
    char ** mod_strs = calloc(MAX_MODS, sizeof(char *));
    size_t mod_count = 0;

    char ** key_strs = calloc(MAX_KEYS, sizeof(char *));
    size_t key_count = 0;

    char * token = strtok(key_group_str, "+");
    while(token) {
        mod_strs[mod_count] = token;
        mod_count++;
        if(mod_count > MAX_MODS) {
            fprintf(stderr, "too many mods");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, "+");
    }

    // Main keys are contained in last mod group
    mod_count--;
    key_group_str = mod_strs[mod_count];

    token = strtok(key_group_str, ":");
    while(token) {
        key_strs[key_count] = token;
        key_count++;
        if(key_count > MAX_KEYS) {
            fprintf(stderr, "too many keys");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, ":");
    }

    new_key_group->mod_count = mod_count;
    for(size_t i = 0; i < mod_count; i++) {
        keycode_t kc = get_keycode(mod_strs[i]);
        if(kc) {
            fprintf(stderr, "translated %s\n", mod_strs[i]);
        } else {
            fprintf(stderr, "could not translate %s\n", mod_strs[i]);
        }
        new_key_group->mods[i] = kc;
    }

    new_key_group->key_count = key_count;
    for(size_t i = 0; i < key_count; i++) {
        keycode_t kc = get_keycode(key_strs[i]);
        if(kc) {
            fprintf(stderr, "translated %s\n", key_strs[i]);
        } else {
            fprintf(stderr, "could not translate %s\n", key_strs[i]);
        }
        new_key_group->keys[i] = kc;
    }

    free(mod_strs);
    free(key_strs);
}

int match_key_group_mod(input_event_t * evt, key_group_t * group) {
    for(size_t i = 0; i < group->mod_count; i++) {
        if(evt->code == group->mods[i])
            return i;
    }
    return -1;
}

int match_key_group_key(input_event_t * evt, key_group_t * group) {
    for(size_t i = 0; i < group->key_count; i++) {
        if(evt->code == group->keys[i])
            return i;
    }
    return -1;
}

int match_event_type(input_event_t * evt, int type) {
    return evt->value == type;
}

bool all_mods_down(key_group_t * group) {
    for(size_t i = 0; i < group->mod_count; i++) {

    }


    return 0;
}
