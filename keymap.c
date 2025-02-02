/* Copyright 2015-2023 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "vendor/kyria-keymap/features/casemodes.h"

enum planck_layers { _QWERTY, _GAMING, _LOWER, _RAISE, _ADJUST };

enum planck_keycodes { QWERTY = SAFE_RANGE, GAMING, BACKLIT, XCASE };

enum { TD_PRN, TD_BRC, TD_CBR, TD_QUOT, TD_GRAV };

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

/* clang-format off */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT_planck_grid(
    KC_TAB,        KC_Q,         KC_W,          KC_E,          KC_R,         KC_T,          KC_Y,          KC_U,         KC_I,         KC_O,         KC_P,            KC_BSPC,
    KC_ESC,        LALT_T(KC_A), LCTL_T(KC_S),  LSFT_T(KC_D),  LGUI_T(KC_F), KC_G,          KC_H,          LGUI_T(KC_J), LSFT_T(KC_K), LCTL_T(KC_L), LALT_T(KC_SCLN), TD(TD_QUOT),
    OSM(MOD_LSFT), KC_Z,         KC_X,          KC_C,          KC_V,         KC_B,          KC_N,          KC_M,         KC_COMM,      KC_DOT,       KC_SLSH,         KC_RSFT,
    OSM(MOD_LCTL), XCASE,        OSM(MOD_LALT), OSM(MOD_LGUI), LOWER,        SFT_T(KC_ENT), SFT_T(KC_SPC), RAISE,        KC_LEFT,      KC_DOWN,      KC_UP,           KC_RGHT
),

// QWERTY layer without the hold modifiers, used when gaming
[_GAMING] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,   KC_W,    KC_E,    KC_R,  KC_T,          KC_Y,          KC_U,  KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_ESC,  KC_A,   KC_S,    KC_D,    KC_F,  KC_G,          KC_H,          KC_J,  KC_K,    KC_L,    KC_SCLN, TD_QUOT,
    KC_LSFT, KC_Z,   KC_X,    KC_C,    KC_V,  KC_B,          KC_N,          KC_M,  KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
    KC_LCTL, QWERTY, KC_LALT, KC_LGUI, LOWER, SFT_T(KC_ENT), SFT_T(KC_SPC), RAISE, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

[_LOWER] = LAYOUT_planck_grid(
    KC_TILD, KC_EXLM,       KC_AT,         KC_HASH,       KC_DLR,        KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, TD(TD_PRN), _______, _______,
    KC_DEL,  LALT_T(KC_F1), LCTL_T(KC_F2), LSFT_T(KC_F3), LGUI_T(KC_F4), KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, TD(TD_CBR), KC_RALT, KC_PIPE,
    _______, KC_F7,         KC_F8,         KC_F9,         KC_F10,        KC_F11,  KC_F12,  _______, _______, TD(TD_BRC), _______, _______,
    _______, _______,       _______,       _______,       _______,       _______, _______, _______, KC_MNXT, KC_VOLD,    KC_VOLU, KC_MPLY
),

[_RAISE] = LAYOUT_planck_grid(
    TD(TD_GRAV), KC_1,          KC_2,          KC_3,          KC_4,          KC_5,    KC_6,    KC_7,            KC_8,    KC_9,       KC_0,    _______,
    KC_DEL,      LALT_T(KC_F1), LCTL_T(KC_F2), LSFT_T(KC_F3), LGUI_T(KC_F4), KC_F5,   KC_F6,   LGUI_T(KC_MINS), KC_EQL,  TD(TD_CBR), KC_RALT, KC_BSLS,
    _______,     KC_F7,         KC_F8,         KC_F9,         KC_F10,        KC_F11,  KC_F12,  _______,         _______, KC_PSCR,    _______, _______,
    _______,     _______,       _______,       _______,       _______,       _______, _______, _______,         KC_HOME, KC_PGDN,    KC_PGUP, KC_END
),

[_ADJUST] = LAYOUT_planck_grid(
    _______, QK_BOOT, DB_TOGG, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______,
    _______, EE_CLR,  MU_NEXT, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, _______, _______, _______, _______, _______,
    _______, AU_PREV, AU_NEXT, MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______,
    _______, GAMING,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)

};
/* clang-format on */

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_case_modes(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case XCASE:
            if (record->event.pressed) {
                enable_xcase();
            }
            return false;
            break;
        case QWERTY:
            if (record->event.pressed) {
                rgblight_disable();
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        case GAMING:
            if (record->event.pressed) {
                rgblight_enable();
                set_single_persistent_default_layer(_GAMING);
            }
            return false;
            break;
        case BACKLIT:
            if (record->event.pressed) {
                register_code(KC_RSFT);
            } else {
                unregister_code(KC_RSFT);
            }
            return false;
            break;
    }
    return true;
}

/* clang-format off */
float melody[8][2][2] = {
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
};
/* clang-format on */

#define JUST_MINOR_THIRD 1.2
#define JUST_MAJOR_THIRD 1.25
#define JUST_PERFECT_FOURTH 1.33333333
#define JUST_TRITONE 1.42222222
#define JUST_PERFECT_FIFTH 1.33333333

#define ET12_MINOR_SECOND 1.059463
#define ET12_MAJOR_SECOND 1.122462
#define ET12_MINOR_THIRD 1.189207
#define ET12_MAJOR_THIRD 1.259921
#define ET12_PERFECT_FOURTH 1.33484
#define ET12_TRITONE 1.414214
#define ET12_PERFECT_FIFTH 1.498307

deferred_token tokens[8];

uint32_t reset_note(uint32_t trigger_time, void *note) {
    *(float*)note = 440.0f;
    return 0;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    cancel_deferred_exec(tokens[index]);
    if (clockwise) {
        melody[index][1][0] = melody[index][1][0] * ET12_MINOR_SECOND;
        melody[index][0][0] = melody[index][1][0] / ET12_PERFECT_FIFTH;
        audio_play_melody(&melody[index], 2, false);
    } else {
        melody[index][1][0] = melody[index][1][0] / ET12_MINOR_SECOND;
        melody[index][0][0] = melody[index][1][0] * ET12_TRITONE;
        audio_play_melody(&melody[index], 2, false);
    }
    tokens[index] = defer_exec(1000, reset_note, &melody[index][1][0]);
    return false;
}

/***************************
 * Double taps             */
typedef struct {
    char open;
    char close;
} tap_dance_braces_t;
typedef struct {
    char quote;
} tap_dance_quotes_t;

#define ACTION_TAP_DANCE_BRACES(bopen, bclose) \
    { .fn = {dance_braces_each, NULL, NULL, NULL}, .user_data = (void *)&((tap_dance_braces_t){bopen, bclose}), }
#define ACTION_TAP_DANCE_QUOTES(quote) \
    { .fn = {dance_quotes_each, NULL, NULL, NULL}, .user_data = (void *)&((tap_dance_quotes_t){quote}), }

void dance_braces_each(tap_dance_state_t *state, void *user_data) {
    tap_dance_braces_t *pair = (tap_dance_braces_t *)user_data;

    if (state->count == 1) {
        if (get_mods() & MOD_MASK_SHIFT) {
            del_mods(MOD_MASK_SHIFT); // disable shift, so it's not sent along with the keycode
            send_char(pair->close);
        } else {
            send_char(pair->open);
        }
    } else {
        send_char(pair->close);
        SEND_STRING(SS_TAP(X_LEFT));
        reset_tap_dance(state);
    }
}

void dance_quotes_each(tap_dance_state_t *state, void *user_data) {
    tap_dance_quotes_t *data = (tap_dance_quotes_t *)user_data;

    if (state->count < 3) {
        send_char(data->quote);
    } else {
        del_mods(MOD_MASK_SHIFT); // disable shift, so it's not sent along with the keycode
        SEND_STRING(SS_TAP(X_LEFT));
        reset_tap_dance(state);
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_PRN] = ACTION_TAP_DANCE_BRACES('(', ')'),
    [TD_CBR] = ACTION_TAP_DANCE_BRACES('{', '}'),
    [TD_BRC] = ACTION_TAP_DANCE_BRACES('[', ']'),
    [TD_QUOT] = ACTION_TAP_DANCE_QUOTES('\''),
    [TD_GRAV] = ACTION_TAP_DANCE_QUOTES('`'),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 200;
        case SFT_T(KC_SPC):
            return TAPPING_TERM - 50;
        default:
            return TAPPING_TERM;
    }
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SFT_T(KC_SPC):
            return true;
        default:
            return false;
    }
}

/***************************
 * Case Mode Functionality */
bool use_default_xcase_separator(uint16_t keycode, const keyrecord_t *record) {
    switch (keycode) {
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
            return true;
    }
    return false;
}
