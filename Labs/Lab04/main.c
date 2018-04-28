#include ".\lab4.h"
#include <AT89LP51.h>
#include <stdlib.h>
#include <8051.h>
#include "ev8031.lib\bitdef.h"
#include "ev8031.lib\ev8031.c"

const unsigned char heart[] = {0x00, 0x0c, 0x1f, 0x1f, 0x0e, 0x04, 0x00, 0x00};
//-----------------------------------------------------------------------------
void store_glyph(uint8_t place, unsigned char[] glyph);
void display_glyph(uint8_t place);
void make_delay16(uint16_t);
//-----------------------------------------------------------------------------
void main(void)
{
    hd_turn_on();
    hd_init();
    make_delay16(160);

    store_glyph(0, heart);
    display_glyph(9, 0, 0);

    while(true) {
    }
}
//-----------------------------------------------------------------------------
void store_glyph(uint8_t place, unsigned char[] glyph) {
    unsigned char cmd = _01000000;
    unsigned char row;
    place \%= 8; // just to be sure
    cmd |= place << 3; // set place in the right bits

    for (row = 0; row < 8; row++) {
        cmd &= 0xf8; // reset low 3 bits
        cmd |= row;
        LCD_CMD = cmd;
        make_delay16(160);
        LCD_DATA = glyph[row];
        make_delay16(160);
    }

    make_delay16(160);
}

void display_glyph(uint8_t x, uint8_t y, uint8_t place) {
    set_cursor(x, y);
    make_delay16(160);
    LCD_DATA = place;
}

void make_delay16(uint16_t millis) {
    uint16_t it;
    for (it = 0; it < millis; it++) {}
}
