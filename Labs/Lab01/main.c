#include ".\Igorek.h"
#include <AT89LP51.h>
#include <stdlib.h>

#include <stdint.h>
//#include <pic16/delay.h>

volatile uint8_t * DISPLAY_DATA = 0x8001;
volatile uint8_t * DISPLAY_PLACE = 0x8002;

void ddelay(uint8_t);

void main(void) {
 uint8_t OCT_DIGITS[] = {
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
    7    // 7
};


    // Repeat 3 times
    // 15 - 32, oct
    const uint8_t HIGH_BEGIN = 3;
    const uint8_t LOW_BEGIN = 2;
    const uint8_t HIGH_END = 1;
    const uint8_t LOW_END = 5;

    const uint8_t DELAY_SMALL = 150;
    const uint8_t DELAY_BIG_REPETITIONS = 100;

	uint8_t rep;
	uint8_t iteration;
    while (1) {
        for (iteration = 0; iteration < 3; iteration++) {
            uint8_t high = HIGH_BEGIN;
            uint8_t low = LOW_BEGIN;

            while (1) {
                for (rep = 0; rep < DELAY_BIG_REPETITIONS; rep++) {
//                for (rep = 0; rep < 1; rep++) {
                    // HIGH
                    *DISPLAY_PLACE = 2;
                    *DISPLAY_DATA = OCT_DIGITS[high];
                    ddelay(DELAY_SMALL);
                    *DISPLAY_DATA = 0;

                    // LOW
                    *DISPLAY_PLACE = 3;
                    *DISPLAY_DATA = OCT_DIGITS[low];
                    ddelay(DELAY_SMALL);
                    *DISPLAY_DATA = 0;
                }

                if (low == 0) {
                    high--;
                    low = 7;
                } else {
					low--;
				}
                if (high == HIGH_END && low == LOW_END) {
                    break;
                }				
            }
        }
		break;
    }
}

void ddelay(uint8_t millis) {
  uint8_t it;
  for (it = 0; it < millis; it++) {}
}
