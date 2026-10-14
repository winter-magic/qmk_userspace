#pragma once
#include "quantum.h"

bool nav_word_enabled(void);
void enable_nav_word(void);
void disable_nav_word(void);
void toggle_nav_word(void);

// In process_record_user aufrufen (wie process_num_word).
bool process_nav_word(uint16_t keycode, keyrecord_t *record);

// Beendet diese Taste Nav Word? Weak, in keymap.c überschreibbar.
bool nav_word_should_terminate(uint16_t keycode, keyrecord_t *record);
