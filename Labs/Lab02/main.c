#include ".\Igorek.h"
#include <AT89LP51.h>
#include <stdlib.h>

#include <stdint.h>

// 16-bit signed, dynamic, negative - #, confirmation - *

volatile uint8_t * DISPLAY_DATA = 0x8001;
volatile uint8_t * DISPLAY_PLACE = 0x8002;

const uint16_t DELAY_FOR_DISPLAY = 150;
const unsigned char NO_KEY_CODE = 12;

uint8_t DIGITS[] = {
    // PGFEDCBA
    // 00111111
    63,  // 0
    // 00000110
    6,   // 1
    // 01011011
    91,  // 2
    // 01001111
    79,  // 3
    // 01100110
    102, // 4
    // 01101101
    109, // 5
    // 01111101
    125, // 6
    // 00000111
    7,   // 7
    // 01111111
    127, // 8
    // 01101111
    111, // 9
    // 00000000
    0,   // empty [10]
    // 01000000
    64   // '-'   [11]
};
//-----------------------------------------------------------------------------
int16_t number = 0;
uint8_t display_current_codes[4] = {DIGITS[10], DIGITS[10], DIGITS[10], DIGITS[10]};


void make_delay16(uint16_t);
void clear_display();
void clear_number();
void display_current();
void process_number();
void get_key();
void push_digit(uint8_t digit);
//-----------------------------------------------------------------------------
void main(void) {
    // main loop
    while (1) {
        display_current();
        unsigned char key = get_key();
        if (key == NO_KEY_CODE) {
            continue;
        }

        // Some valid key got pressed
        switch (key) {
            case 10: // *
                process_number();
                break;
            case 11: // #
                push_digit(DIGITS[11]);
                number *= -1;
                break;
            default:
                push_digit(DIGITS[key]);
                number *= 10;
                number += key;
                break;
        }
    }
}
//-----------------------------------------------------------------------------
void make_delay16(uint16_t millis) {
    uint16_t it;
    for (it = 0; it < millis; it++) {}
}

void display_current() {
    uint8_t i;
    for (i = 0; i < 4; i++) {
        *DISPLAY_PLACE = i;
        *DISPLAY_DATA = display_current_codes[i];
        make_delay16(DELAY_FOR_DISPLAY);
        *DISPLAY_DATA = 0;
    }
}

void clear_display() {
    uint8_t i;
    for (i = 0; i < 4; i++) {
        display_current_codes[i] = DIGITS[10];
    }
}

void clear_number() {
    number = 0;
}

unsigned char get_key() {
    unsigned char previous_scan = key_scan();
    if (previous_scan == 12) { // no key
        return NO_KEY_CODE;
    }

    // Make a delay of ~12ms
    const uint8_t DELAY = 10;
    uint8_t i;
    for (i = 0; i < DELAY; i++) {
        display_current(); // ~600 ops
    }

    unsigned char new_scan = key_scan();
    if (previous_scan == new_scan) {
        do {
            display_current();
            new_scan = key_scan(); // wait for user to release the key
        } while (new_scan != 12);

        return previous_scan;
    } else {
        return NO_KEY_CODE;
    }
}

void process_number() {
    clear_display();
    clear_number();
}

void push_digit(uint8_t digit) {
    uint8_t i;
    for (i = 1; i < 4; i++) {
        display_current_codes[i - 1] = display_current_codes[i];
    }
    display_current_codes[3] = digit;
}
//-----------------------------------------------------------------------------
