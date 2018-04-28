#include ".\lab3.h"
#include <AT89LP51.h>
#include <stdlib.h>
#include <8051.h>
#include "ev8031.lib\bitdef.h"
#include "ev8031.lib\ev8031.c"

const unsigned char NO_KEY_CODE = 12;
const char HEX_DIGITS[] = {
    0x30,
    0x31,
    0x32,
    0x33,
    0x34,
    0x35,
    0x36,
    0x37,
    0x38,
    0x39,
    0x41,
    0x42,
    0x43,
    0x44,
    0x45,
    0x46
};
//-----------------------------------------------------------------------------
void make_delay16(uint16_t);
void get_key();
void display_char(unsigned char code);
void display_char(uint8_t x, uint8_t y, unsigned char code);
void display_number(uint8_t y, uint16_t number);
uint8_t get_amount_of_digits(uint16_t number);
void clear_row(uint8_t row);
//-----------------------------------------------------------------------------
void main(void)
{
    unsigned char key;
    uint16_t press_counter;
    bool part_1_done;
    bool part_2_done;
    uint16_t current_number;

    hd_turn_on();
    hd_init();
    make_delay16(160);

    while(true) {
        press_counter = 0;
        part_1_done = 0; // false
        part_2_done = 0; // false
        current_number = 0;

        clear_row(3);
        display_number(3, press_counter);
        while (!part_1_done) {
            key = get_key();
            if (key == NO_KEY_CODE) {
                continue;
            }
            if (key == 15) {
                press_counter++;
                display_number(3, press_counter);
            } else if (key == 0) {
                part_1_done = 1; // true
            }
        }

        clear_row(0);
        display_number(0, current_number);
        while (!part_2_done) {
            display_number(0, current_number);
            key = get_key();
            if (key == NO_KEY_CODE) {
                continue;
            }
            switch (key) {
                case 10:
                    part_2_done = 1; // true;
                    break;
                default:
                    current_number *= 10;
                    current_number += key;
                    display_number(0, current_number);
                    break;
            }
        }
    }
}
//-----------------------------------------------------------------------------
void make_delay16(uint16_t millis) {
    uint16_t it;
    for (it = 0; it < millis; it++) {}
}

unsigned char get_key() {
    const uint8_t DELAY = 10;
    uint8_t i;
    unsigned char new_scan;
    unsigned char previous_scan;

    previous_scan = key_scan();
    if (previous_scan == 12) { // no key
        return NO_KEY_CODE;
    }

    // Make a delay of ~12ms
    for (i = 0; i < DELAY; i++) {
        display_current(); // ~600 ops
    }

    new_scan = key_scan();
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

void display_char(unsigned char code) {
    make_delay16(160);
    LCD_DATA = code;
}

void display_char(uint8_t x, uint8_t y, unsigned char code) {
    make_delay16(160);
    set_cursor(x, y);
    make_delay16(160);
    LCD_DATA = code;
}

void display_number(uint8_t y, uint16_t number) {
    uint16_t digit;
    uint8_t x;

    x = 10 - get_amount_of_digits(number);

    make_delay16(160);
    set_cursor(x, y);
    while (number > 0) {
        digit = number % 10;
        number /= 10;
        display_char(HEX_DIGITS[digit]);
    }
}

uint8_t get_amount_of_digits(uint16_t number) {
    uint8_t counter;
    while (number > 0) {
        number /= 10;
        cunter++;
    }

    return counter;
}

void clear_row(uint8_t row) {
    uint8_t i;

    make_delay16(160);
    set_cursor(i, row);
    for (i = 0; i < 10; i++) {
        display_char(0x20); // empty char
    }
}
