// Netable differences vs. the default firmware for the ErgoDox EZ:
// 1. The Cmd key is now on the right side, making Cmd+Space easier.
// 2. The media keys work on OSX (But not on Windows).
#include <stdarg.h>
#include "ergodox.h"
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "eeconfig.h"
#include "wait.h"
#include "process_unicode.h"

// Keycodes
#define LCHG LGUI(KC_SPC)
#define MCRN UC(0x00fa)

/* Layers */
enum {
  BASE = 0,  // default layer
  SYMB,      // symbols
  MDIA,      // media keys
  DVRK,      // Dvorak layer
};

enum extended_keycodes {
  LIN = SAFE_RANGE,
  WIN,
  OSX,
};

/* Custom keycodes */
enum {
  CT_CLN = 0,
  CT_LBP,
  CT_RBP,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   DVRK |   1  |   2  |   3  |   4  |   5  |  Del |           | Ins  |   6  |   7  |   8  |   9  |   0  |  POWER |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |   =  |           |  -   |   Y  |   U  |   I  |   O  |   P  |   \|   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |Ctl/⌘+sp|   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |   '"   |
 * |--------+------+------+------+------+------|Leader|           | Meh  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |  ~L1 |  Grv | Caps | Left | Right|                                       | Down |  Up  |   [  |   ]  | ~L2  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | LCtl | LGui |       | LCtl | LGui |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | LAlt |       | LAlt |      |      |
 *                                 | Bkspc| Esc  |------|       |------|Enter |Space |
 *                                 |      |      | ~L2  |       |  ~L1 |      |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
            KC_FN3,  KC_1,   KC_2,     KC_3,   KC_4, KC_5,   KC_DELETE,
            KC_TAB,  KC_Q,   KC_W,     KC_E,   KC_R, KC_T,   KC_EQL,
            LCHG,    KC_A,   KC_S,     KC_D,   KC_F, KC_G,
            KC_LSPO, KC_Z,   KC_X,     KC_C,   KC_V, KC_B,   KC_LEAD, //ALL_T(KC_NO),
            KC_FN1,  KC_GRV, KC_CAPS, KC_LEFT,KC_RGHT,
                                               KC_LCTL, KC_LGUI,
                                                        KC_LALT,
                                      KC_BSPC, KC_ESC,  KC_FN2,
        // right hand
             KC_INS,      KC_6,   KC_7,   KC_8,   KC_9,   KC_0,       KC_POWER,
             KC_MINS,     KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,       KC_BSLS,
                          KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,    KC_QUOT,
             MEH_T(KC_NO),KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,    KC_RSPC,
                                  KC_DOWN,KC_UP,KC_LBRC,KC_RBRC,      KC_FN2,
             KC_LCTL,        KC_LGUI,
             KC_LALT,
             KC_FN1,KC_ENT, KC_SPC
    ),
/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  | F11  |           | F12  |  F6  |  F7  |  F8  |  F9  |  F10 |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |        |
 * |--------+------+------+------+------+------| Home |           | PgUp |------+------+------+------+------+--------|
 * |        |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |--------+------+------+------+------+------|  End |           | PgDn |------+------+------+------+------+--------|
 * |        |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |    . |   0  |   =  |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = KEYMAP(
       // left hand
       KC_TRNS,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F11,
       KC_TRNS,KC_EXLM,KC_AT,  KC_LCBR,KC_RCBR,KC_PIPE,KC_HOME,
       KC_TRNS,KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
       KC_TRNS,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_TILD,KC_END,
       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_F12, KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_TRNS,
       KC_PGUP, KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_TRNS,
                KC_DOWN, KC_4,   KC_5,    KC_6,    KC_PLUS, MCRN,
       KC_PGDN, KC_AMPR, KC_1,   KC_2,    KC_3,    KC_BSLS, KC_TRNS,
                         KC_TRNS,KC_DOT,  KC_0,    KC_EQL,  KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 2: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        | LIN  | WIN  | OSX  |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      | Rclk | MsUp | Lclk | Mclk |      |           |      |      | Mclk |MsWhUp|      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------| MsWhL| Lclk |MsWhDn| Rclk | MsWhR|  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |VolDn |VolUp | Mute |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MDIA] = KEYMAP(
       KC_TRNS, LIN,     WIN,     OSX,     KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_BTN2, KC_MS_U, KC_BTN1, KC_BTN3, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_BTN3, KC_WH_U, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_WH_L, KC_BTN1, KC_WH_D, KC_BTN2, KC_WH_R, KC_MPLY,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
                          KC_VOLD, KC_VOLU, KC_MUTE, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
),
/* Keymap 3: Dvorak layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   "  |   ,  |   .  |   P  |   Y  |   (  |           |  )   |   F  |   G  |   C  |   R  |   L  |   \|   |
 * |--------+------+------+------+------+------|   [  |           |  ]   |------+------+------+------+------+--------|
 * |        |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |   S  |   -    |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * |        |   :  |   Q  |   J  |   K  |   X  |      |           |      |   B  |   M  |   W  |   V  |   Z  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |  ~L1 |  Grv | Caps | Left | Right|                                       | Down |  Up  |   =  |   /  | ~L2  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | LCtl | LGui |       | LCtl | LGui |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | LAlt |       | LAlt |      |      |
 *                                 | Bkspc| Esc  |------|       |------|Enter |Space |
 *                                 |      |      | ~L2  |       |  ~L1 |      |      |
 *                                 `--------------------'       `----------------------'
 */
[DVRK] = KEYMAP(  // layer 3 : default
        // left hand
                KC_TRNS,  KC_TRNS,   KC_TRNS,    KC_TRNS,   KC_TRNS, KC_TRNS,   KC_TRNS,
                KC_TRNS,  KC_QUOT,   KC_COMM,     KC_DOT,   KC_P, KC_Y,   TD(CT_LBP),
                KC_TRNS,    KC_A,   KC_O,     KC_E,   KC_U, KC_I,
                KC_TRNS, KC_SCLN,   KC_Q,     KC_J,   KC_K, KC_X,   KC_TRNS,
                KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,KC_TRNS,
                                               KC_TRNS, KC_TRNS,
                                                        KC_TRNS,
                                      KC_TRNS, KC_TRNS, KC_TRNS,
        // right hand
                KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                TD(CT_RBP), KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_TRNS,
                            KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_MINS,
                KC_TRNS,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_RSPC,
                    KC_TRNS,KC_TRNS,KC_EQL,KC_SLSH,      KC_TRNS,
                KC_TRNS,        KC_TRNS,
                KC_TRNS,
                KC_TRNS,KC_TRNS, KC_TRNS
    ),
};

bool process_record_user (uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LIN:
      set_unicode_input_mode(UC_LNX);
      return false;
      break;
    case WIN:
      set_unicode_input_mode(UC_WIN);
      return false;
      break;
    case OSX:
      set_unicode_input_mode(UC_OSX);
      return false;
      break;
  }
  return true;
}

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB),          // FN1 - Momentary Layer 1 (Symbols)
    [2] = ACTION_LAYER_TAP_TOGGLE(MDIA),          // FN2 - Momentary Layer 2 (Media)
    [3] = ACTION_LAYER_TAP_TOGGLE(DVRK),          // FN3 - Momentary Layer 3 (Dvorak)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
        if (record->event.pressed) {
          register_code(KC_RSFT);
        } else {
          unregister_code(KC_RSFT);
        }
        break;
      }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    set_unicode_input_mode(UC_OSX);
};

LEADER_EXTERNS();

qk_tap_dance_action_t tap_dance_actions[] = {
  [CT_CLN] = ACTION_TAP_DANCE_DOUBLE (KC_COLN, KC_SCLN),
  [CT_LBP] = ACTION_TAP_DANCE_DOUBLE (KC_LBRC, KC_LPRN),
  [CT_RBP] = ACTION_TAP_DANCE_DOUBLE (KC_RBRC, KC_RPRN)
};

#define TAP_ONCE(code)  \
  register_code (code); \
  unregister_code (code)

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        case 3:
            ergodox_right_led_3_on();
            break;
        default:
            // none
            break;
    }

    LEADER_DICTIONARY() {
        leading = false;
        leader_end ();

        SEQ_ONE_KEY (KC_S) {
            unicode_input_start(); register_hex(0xaf); unicode_input_finish();
            TAP_ONCE (KC_BSLS);
            register_code (KC_RSFT); TAP_ONCE (KC_MINS); TAP_ONCE (KC_9); unregister_code (KC_RSFT);
            unicode_input_start (); register_hex(0x30c4); unicode_input_finish();
            register_code (KC_RSFT); TAP_ONCE (KC_0); TAP_ONCE (KC_MINS); unregister_code (KC_RSFT);
            TAP_ONCE (KC_SLSH);
            unicode_input_start (); register_hex(0xaf); unicode_input_finish();
        }
    }

};
