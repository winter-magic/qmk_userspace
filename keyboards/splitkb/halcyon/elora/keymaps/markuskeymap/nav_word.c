/* Minimal standalone "Nav Word" für QMK, nach dem Muster von num_word.c.
 *
 * Nav Word hält den _NAV-Layer dauerhaft an. Es endet, sobald eine Taste aus der
 * Ausstiegsliste (nav_word_should_terminate) getippt wird. Die Taste wird dabei
 * ganz normal gesendet.
 */

#include "nav_word.h"
#include "layers.h"

#define NAV_WORD_LAYER _NAV

static bool nav_word_on = false;

bool nav_word_enabled(void) {
    return nav_word_on;
}

void enable_nav_word(void) {
    if (nav_word_on) return;
    nav_word_on = true;
    layer_on(NAV_WORD_LAYER);
}

void disable_nav_word(void) {
    if (!nav_word_on) return;
    nav_word_on = false;
    layer_off(NAV_WORD_LAYER);
}

void toggle_nav_word(void) {
    if (nav_word_on) {
        disable_nav_word();
    } else {
        enable_nav_word();
    }
}

// Ausstiegsliste: Diese Tasten beenden Nav Word (und werden normal gesendet).
// Alles andere (Pfeile, Home/End, F2, ...) lässt Nav Word aktiv.
// Bei Mod-Tap/Layer-Tap ist keycode schon der Tap-Keycode.
__attribute__((weak)) bool nav_word_should_terminate(uint16_t keycode, keyrecord_t *record) {
    // Reine Modifier beenden nie, egal ob einzeln oder als Teil eines Mod-Taps gehalten.
    if (IS_MODIFIER_KEYCODE(keycode)) {
        return false;
    }

    // Editieren beim Navigieren
    if (keycode == KC_BSPC || keycode == KC_DEL) {
        return false;
    }

    switch (keycode) {
        case KC_A ... KC_SLASH: // Buchstaben, Ziffern, Esc, Enter, Tab, Space, Satzzeichen
        case KC_NUBS:           // < > |
        case KC_F23:
            return true;
        default:
            return false;
    }
}

bool process_nav_word(uint16_t keycode, keyrecord_t *record) {
    if (!nav_word_on) {
        return true;
    }
    if (record->event.pressed) {
        switch (keycode) {
            case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
#ifndef NO_ACTION_TAPPING
                if (record->tap.count == 0) {
                    // Taste wird nur als Layer-Halte-Taste genutzt ->
                    // echter Layerwechsel -> Nav Word beenden.
                    disable_nav_word();
                    return true;
                }
#endif
                keycode = keycode & 0xFF;
                break;

            case QK_MOD_TAP ... QK_MOD_TAP_MAX:
#ifndef NO_ACTION_TAPPING
                if (record->tap.count == 0) {
                    return true; // gehalten (Modifier) -> durchlassen
                }
#endif
                keycode = keycode & 0xFF;
                break;

            default:
                break;
        }
        if (nav_word_should_terminate(keycode, record)) {
            disable_nav_word();
        }
    }
    return true;
}
