/* Minimal standalone "Num Word" for QMK.
 * Adapted/stripped down from vvhg1's casemodes.c (itself adapting
 * Joshua T.'s NumWord concept). See num_word.h for setup instructions.
 *
 * Original casemodes.c:
 * Copyright 2021 Andrew Rae ajrae.nv@gmail.com @andrewjrae,
 * NumWord by Joshua T., adaption by 2022 @vvhg1
 * Licensed GPL-2.0-or-later.
 */

#include "num_word.h"
#include "keymap_german.h"

#define DE_PLUS KC_RBRC
#define DE_MINUS KC_SLSH
#define DE_MAL LSFT(KC_RBRC)
#define DE_FSLASH LSFT(KC_7)


static bool num_word_on = false;

bool num_word_enabled(void) {
    return num_word_on;
}

void enable_num_word(void) {
    if (num_word_on) return;
    num_word_on = true;
    layer_on(NUM_WORD_LAYER);
}

void disable_num_word(void) {
    if (!num_word_on) return;
    num_word_on = false;
    layer_off(NUM_WORD_LAYER);
}

void toggle_num_word(void) {
    if (num_word_on) {
        disable_num_word();
    } else {
        enable_num_word();
    }
}

// Default set of keys that DON'T end Num Word. Override this in your
// keymap by defining your own num_word_should_terminate() - just
// don't mark it `weak` there, and remove/rename this one if you do.
__attribute__((weak)) bool num_word_should_terminate(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Top-row and shifted-top-row numbers
        case KC_1 ... KC_0:
        // Numpad
        case KC_P1 ... KC_P0:
        case KC_PSLS ... KC_PPLS:
        case KC_PDOT:
        // Common "stays in a number" punctuation
        case KC_DOT:
        case KC_MINS:
        case KC_UNDS:
        case KC_EQL:
        case KC_SCLN:
        case DE_COMM:
        case DE_FSLASH:
        case DE_MAL:
        case DE_MINUS:
        case DE_PLUS:
        // Editing while typing a number
        case KC_BSPC:
        case KC_DEL:
            return false; // allowed, Num Word stays active
        default:
            if (record->event.pressed) {
                return true; // anything else ends Num Word
            }
            return false; // ignore key-up events
    }
}

bool process_num_word(uint16_t keycode, keyrecord_t *record) {
    if (!num_word_on) {
        return true;
    }

    if (record->event.pressed) {
        // Resolve mod-tap / layer-tap / tap-dance keys to their tapped
        // keycode so e.g. LT(1, KC_1) is treated as KC_1.
        switch (keycode) {
            case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
#ifndef NO_ACTION_TAPPING
                if (record->tap.count == 0) {
                    return true; // held, not tapped - let it through
                }
#endif
                keycode = keycode & 0xFF;
                break;
            default:
                break;
        }

        if (num_word_should_terminate(keycode, record)) {
            disable_num_word();
        }
    }

    return true;
}
