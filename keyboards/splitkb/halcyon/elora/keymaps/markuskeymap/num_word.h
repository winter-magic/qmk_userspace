/* Minimal standalone "Num Word" for QMK.
 * Adapted/stripped down from vvhg1's casemodes.c (itself adapting
 * Joshua T.'s NumWord concept), removing Caps Word and X-Case.
 *
 * Usage:
 *   1. Copy num_word.c and num_word.h into your keymap folder
 *      (e.g. keymaps/<you>/features/).
 *   2. Add num_word.c to your keymap's rules.mk:
 *        SRC += features/num_word.c
 *   3. #include "features/num_word.h" in your keymap.c
 *   4. Define a _NUM layer in your `enum layers` with numbers/symbols
 *      on the keys you want available while Num Word is active.
 *   5. Call toggle_num_word() from a custom keycode, combo, tap dance,
 *      etc. in process_record_user().
 *   6. Call process_num_word(keycode, record) at the top of your
 *      process_record_user(), and respect its return value:
 *
 *        bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 *            if (!process_num_word(keycode, record)) {
 *                return false;
 *            }
 *            // ... your other keycode handling ...
 *            return true;
 *        }
 */

#pragma once

#include "quantum.h"
#include "layers.h" 

// Replace _NUM with the name of your numbers layer.
#ifndef NUM_WORD_LAYER
#    define NUM_WORD_LAYER _NUM
#endif

bool num_word_enabled(void);
void enable_num_word(void);
void disable_num_word(void);
void toggle_num_word(void);

// Call this from process_record_user(). Returns false if the keypress
// was consumed and no further processing should happen (currently
// always returns true, but kept for API symmetry / future use).
bool process_num_word(uint16_t keycode, keyrecord_t *record);

// Weak function you can override in your keymap to customize which
// keycodes keep Num Word active (return true = key breaks/ends Num
// Word, false = key is allowed and Num Word stays on).
bool num_word_should_terminate(uint16_t keycode, keyrecord_t *record);
