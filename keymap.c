/*
 * I used Kinesis Advantage keyboard before and I like modifiers' placement on that keyboard.
 * */
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
#define UC_CMD UC(0x2318)

/* Layers */
enum my_layers {
  BASE = 0,  // default layer
  SYMB,      // symbols
  MDIA,      // media keys
  ARRW,      // arrow layer
  APPS,      // applications layer
};

enum extended_keycodes {
  LIN = SAFE_RANGE,
  WIN,
  OSX,
};

/* Custom keycodes */
enum tap_dance_keycodes {
  CT_CLN = 0,
  CT_LBP,
  CT_RBP,
  CT_TA,
};

/* Fn keys */
enum fn_keys {
  F_BSE = 0,

  SYMB_TG,
  MDIA_TG,
  SYMB_OS,
  MDIA_OS,

  F_CTL,
  F_ALT,
  F_GUI,
  F_SFT,
  F_ENT,
};

/* Macros */

enum my_macroses {
  NONE = 0,
  A_GUI,
  APP_SFRI,
  APP_SLK,
  APP_EMCS,
  APP_TERM,
  APP_CHRM,
  APP_TLGRM,
  APP_MAIL,
  APP_PSTBX,
  A_ENT,
};

uint16_t gui_timer = 0;

uint16_t kf_timers[12];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |  `~    |   1  |   2  |   3  |   4  |   5  |  Del |           | Ins  |   6  |   7  |   8  |   9  |   0  |  POWER |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |   [  |           |  ]   |   Y  |   U  |   I  |   O  |   P  |   \|   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | ⌘ + sp |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |' / ARRW|
 * |--------+------+------+------+------+------|   -  |           |  =   |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | SYMB |      |      |      |  `~  |                                       |  \|  |      |      |      | MDIA |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | LCtl | LGui |       | Lead | LGui |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | LAlt |       | LAlt |      |      |
 *                                 | Bkspc| Esc  |------|       |------|Enter |Space |
 *                                 |      |      | MDIA |       | SYMB |      |      |
 *                                 `--------------------'       `--------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
            KC_GRV,     KC_1,   KC_2,    KC_3,  KC_4,  KC_5,   KC_DELETE,
            KC_TAB,     KC_Q,   KC_W,    KC_E,  KC_R,  KC_T,   TD(CT_LBP),
            TD(CT_TA),  KC_A,   KC_S,    KC_D,  KC_F,  KC_G,
            F(F_SFT),   KC_Z,   KC_X,    KC_C,  KC_V,  KC_B,   KC_EQL, //ALL_T(KC_NO),
            F(SYMB_TG), KC_NO,  KC_NO, KC_NO, KC_GRV,
                                               F(F_CTL),F(F_GUI),
                                                        F(F_ALT),
                                      KC_BSPC, KC_ESC,  F(MDIA_OS),
        // right hand
             KC_INS,    KC_6,   KC_7,   KC_8,   KC_9,  KC_0,   KC_POWER,
             TD(CT_RBP),KC_Y,   KC_U,   KC_I,   KC_O,  KC_P,   KC_BSLS,
                        KC_H,   KC_J,   KC_K,   KC_L,  KC_SCLN,LT(ARRW, KC_QUOT),
             KC_MINS,   KC_N,   KC_M,   KC_COMM,KC_DOT,CTL_T(KC_SLSH),F(F_SFT),
                                  KC_BSLS,KC_NO,KC_NO,KC_NO,      F(MDIA_TG),
             KC_LEAD,        F(F_GUI),
             F(F_ALT),
             F(SYMB_OS),F(F_ENT), KC_SPC
    ),
/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  | F11  |           | F12  |  F6  |  F7  |  F8  |  F9  |  F10 |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   @  |   {  |   }  |   |  |      |           |      |  -   |   7  |   8  |   9  |   *  |        |
 * |--------+------+------+------+------+------| Home |           | PgUp |------+------+------+------+------+--------|
 * |  UC(⌘ )|   #  |   $  |   (  |   )  |   `  |------|           |------|  _   |   4  |   5  |   6  |   +  |        |
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
       UC_CMD, KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
       KC_TRNS,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_TILD,KC_END,
       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_F12, KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_TRNS,
       KC_PGUP,KC_MINS,KC_7,   KC_8,   KC_9,   KC_ASTR,KC_TRNS,
               KC_UNDS,KC_4,   KC_5,   KC_6,   KC_PLUS,KC_TRNS,
       KC_PGDN,KC_AMPR,KC_1,   KC_2,   KC_3,   KC_BSLS,KC_TRNS,
                       KC_TRNS,KC_DOT, KC_0,   KC_EQL, KC_TRNS,
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
/* Keymap 3: layer with arrow keys and unicode symbols
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      | Home | PgDn | PgUp |  End |      |           |      | Home | PgDn | PgUp |  End |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |  ←   |  ↓   |  ↑   |  →   |------|           |------|  ←   |  ↓   |  ↑   |  →   |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// Arrow keys
[ARRW] = KEYMAP(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_TRNS, KC_TRNS,
                 KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
    ),
/* Keymap 4: Application select layer
 *
 * ,-----------------------------------------------------.       ,-----------------------------------------------------.
 * |           |Safari|Slack |Emacs |Term  |Chrome|      |       |      |Tlgrm | Mail | Pbox |      |      |           |
 * |-----------+------+------+------+------+-------------|       |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |       |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |       |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|       |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |       |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |       |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'       `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                   |      |      |      |      |      |
 *      `----------------------------------'                                   `----------------------------------'
 *                                         ,-------------.       ,-------------.
 *                                         |      |      |       |      |      |
 *                                  ,------|------|------|       |------+------+------.
 *                                  |      |      |      |       |      |      |      |
 *                                  |      |      |------|       |------|      |      |
 *                                  |      |      |      |       |      |      |      |
 *                                  `--------------------'       `--------------------'
 */

[APPS] = KEYMAP(
      // left hand
      KC_TRNS, M(APP_SFRI),M(APP_SLK),M(APP_EMCS),M(APP_TERM),M(APP_CHRM),KC_TRNS,
      KC_TRNS, KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
      KC_TRNS, KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,
      KC_TRNS, KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
      KC_TRNS, KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,

                                                  KC_TRNS, KC_TRNS,
                                                            KC_TRNS,
                                          KC_TRNS ,KC_TRNS, KC_TRNS,

      // right hand
      KC_TRNS, M(APP_TLGRM),M(APP_MAIL),M(APP_PSTBX),KC_NO,   KC_NO,   KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,    KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS,
               KC_TRNS,     KC_TRNS,    KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,    KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS,
                            KC_TRNS,    KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS,

      KC_TRNS ,KC_TRNS,
      KC_TRNS,
      KC_TRNS ,KC_TRNS  ,KC_TRNS
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
    case KC_ESC:
      if (record->event.pressed) {
        bool queue = true;

        if ((get_oneshot_mods()) && !has_oneshot_mods_timed_out()) {
          clear_oneshot_mods();
          queue = false;
        }

        return queue;
      }
      break;
  }
  return true;
}

const uint16_t PROGMEM fn_actions[] = {
    [SYMB_TG] = ACTION_LAYER_TAP_TOGGLE(SYMB),          // FN1 - Momentary Layer 1 (Symbols)
    [MDIA_TG] = ACTION_LAYER_TAP_TOGGLE(MDIA),          // FN2 - Momentary Layer 2 (Media)
    [SYMB_OS] = ACTION_LAYER_ONESHOT(SYMB),
    [MDIA_OS] = ACTION_LAYER_ONESHOT(MDIA),
    [F_CTL] = ACTION_MODS_ONESHOT(MOD_LCTL),
    [F_ALT] = ACTION_MODS_ONESHOT(MOD_LALT),
    [F_GUI] = ACTION_MACRO_TAP(A_GUI),
    [F_SFT] = ACTION_MODS_ONESHOT(MOD_LSFT),
    [F_ENT] = ACTION_MACRO_TAP(A_ENT),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      uint16_t kc = KC_ENT;
      switch(id) {

        case NONE:
          if (record->event.pressed) {
            register_code(KC_RSFT);
          } else {
            unregister_code(KC_RSFT);
          }
          break;

        case A_ENT:
          if (record->event.pressed) {
            kc = KC_LGUI;
            if (record->tap.count && !record->tap.interrupted) {
              kc = KC_ENT;
              register_code(kc);
            } else {
              register_code(kc);
              gui_timer = 0;
            }
          } else {
            gui_timer = timer_read();
            if (kc == KC_ENT) {
              unregister_code(kc);
            }
          }
          break;

        case A_GUI:
          if (record->event.pressed) {
            register_code (KC_LGUI);
            if (record->tap.count && !record->tap.interrupted) {
              if (record->tap.count >= 2) {
                uprintf("CMD:appsel_alfred\n");
                layer_on (APPS);
                set_oneshot_layer (APPS, ONESHOT_START);
              }
            } else {
              record->tap.count = 0;
            }
            gui_timer = 0;
          } else {
            if (record->tap.count >= 2) {
              clear_oneshot_layer_state (ONESHOT_PRESSED);
            }
            gui_timer = timer_read ();
          }
          break;

        case APP_SFRI:
          if (record->event.pressed)
            uprintf("CMD:appsel_web\n");
          break;

        case APP_SLK:
          if (record->event.pressed)
            uprintf("CMD:appsel_slack\n");
          break;

        case APP_EMCS:
          if (record->event.pressed)
            uprintf("CMD:appsel_emacs\n");
          break;

        case APP_TERM:
          if (record->event.pressed)
            uprintf("CMD:appsel_term\n");
          break;

        case APP_CHRM:
          if (record->event.pressed)
            uprintf("CMD:appsel_chrome\n");
          break;

        case APP_TLGRM:
          if (record->event.pressed)
            uprintf("CMD:appsel_telegram\n");
          break;

        case APP_MAIL:
          if (record->event.pressed)
            uprintf("CMD:appsel_mail\n");
          break;

        case APP_PSTBX:
          if (record->event.pressed)
            uprintf("CMD:appsel_postbox\n");
          break;

      }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    set_unicode_input_mode(UC_OSX);
};

LEADER_EXTERNS();

typedef struct {
  bool layer_toggle;
  bool sticky;
} td_ta_state_t;

static void drg_tap (uint16_t code, ...) {
  uint16_t kc = code;
  va_list ap;

  va_start(ap, code);

  do {
    register_code16(kc);
    unregister_code16(kc);
    wait_ms(50);
    kc = va_arg(ap, int);
  } while (kc != 0);
  va_end(ap);
}

static void drg_tap_dance_ta_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (td_ta->sticky) {
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
    layer_off (ARRW);
    return;
  }

  if (state->count == 1 && !state->pressed) {
    register_code (KC_LGUI);
    register_code (KC_SPC);
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
  } else {
    td_ta->layer_toggle = true;
    layer_on (ARRW);
    td_ta->sticky = (state->count == 2);
  }
}

static void drg_tap_dance_ta_reset (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (!td_ta->layer_toggle) {
    unregister_code (KC_SPC);
    unregister_code (KC_LGUI);
  }
  if (!td_ta->sticky)
    layer_off (ARRW);
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [CT_CLN] = ACTION_TAP_DANCE_DOUBLE (KC_COLN, KC_SCLN),
  [CT_LBP] = ACTION_TAP_DANCE_DOUBLE (KC_LBRC, KC_LPRN),
  [CT_RBP] = ACTION_TAP_DANCE_DOUBLE (KC_RBRC, KC_RPRN),
  [CT_TA]  = {
    .fn = { NULL, drg_tap_dance_ta_finished, drg_tap_dance_ta_reset },
    .user_data = (void *)&((td_ta_state_t) { false, false })
  },
};

#define TAP_ONCE(code)  \
  register_code (code); \
  unregister_code (code)

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    if (gui_timer && timer_elapsed (gui_timer) > TAPPING_TERM)
      unregister_code (KC_LGUI);

    ergodox_board_led_off();
    ergodox_right_led_1_off(); ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        default:
            // none
            break;
    }

    LEADER_DICTIONARY() {
        leading = false;
        leader_end ();
        uint8_t unic_mode = get_unicode_input_mode();

        /* One-key sequences */

        SEQ_ONE_KEY (KC_S) {
            unicode_input_start(); register_hex(0xaf); unicode_input_finish();
            TAP_ONCE (KC_BSLS);
            register_code (KC_RSFT); TAP_ONCE (KC_MINS); TAP_ONCE (KC_9); unregister_code (KC_RSFT);
            unicode_input_start (); register_hex(0x30c4); unicode_input_finish();
            register_code (KC_RSFT); TAP_ONCE (KC_0); TAP_ONCE (KC_MINS); unregister_code (KC_RSFT);
            TAP_ONCE (KC_SLSH);
            unicode_input_start (); register_hex(0xaf); unicode_input_finish();
        }

        SEQ_ONE_KEY (KC_P) {
            if ((unic_mode == UC_LNX) || (unic_mode == UC_WIN)) {
                drg_tap (LSFT(KC_INS), 0);
            } else if (unic_mode == UC_OSX) {
                drg_tap (LGUI(KC_V), 0);
            }
        }

        SEQ_ONE_KEY (KC_L) {
            drg_tap (LSFT(LCTL(KC_POWER)), 0);
        }

        /* Two-key sequences: UI */

        SEQ_TWO_KEYS (KC_W, KC_D) { // close current window
            if ((unic_mode == UC_LNX) || (unic_mode == UC_WIN)) {
                drg_tap (LALT(KC_F4), 0);
            } else if (unic_mode == UC_OSX) {
                drg_tap (LGUI(KC_W), 0);
            }
        }

        SEQ_TWO_KEYS (KC_W, KC_F) { // full-screen (osx only)
            drg_tap (LGUI(LCTL(KC_F)), 0);
        }

        /* Two-key sequences: Applications
         * All of these commands depends on hid-commands.sh implementation*/

        SEQ_TWO_KEYS (KC_A, KC_S) { // Slack
            uprintf("CMD:appsel_slack\n");
        }

        SEQ_TWO_KEYS (KC_A, KC_M) { // Mail
            uprintf("CMD:appsel_mail\n");
        }

        SEQ_TWO_KEYS (KC_A, KC_T) { // Telegram
            uprintf("CMD:appsel_telegram\n");
        }

        SEQ_TWO_KEYS (KC_A, KC_W) { // Default web-browser
            uprintf("CMD:appsel_web\n");
        }

        SEQ_TWO_KEYS (KC_A, KC_G) { // Google Chrome
            uprintf("CMD:appsel_chrome\n");
        }

        SEQ_TWO_KEYS (KC_A, KC_I) { // Terminal
            uprintf("CMD:appsel_term\n");
        }

        SEQ_TWO_KEYS (KC_A, KC_A) {
            uprintf("CMD:appsel_alfred\n");
        }

    }

};
