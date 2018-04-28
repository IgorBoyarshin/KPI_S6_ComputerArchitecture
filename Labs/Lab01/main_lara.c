#include <avr/stdin.h>
#include <avr/delay.h>

volatile uint8_t * dyn_data = 0x8001;
volatile uint8_t * dyn_place = 0x8002;

const uint8_t[] DIGITS = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

void main(void) {
    while (1) {
        for (uint8_t i = 0; i < 3; i++) {
            for (uint8_t value = 0; value <= 20; value++) {
                const uint8_t high = value / 10;
                const uint8_t low = value % 10;
                for (uint8_t delay = 0; delay < 500; delay++) {
                    // HIGH
                    *dyn_place = 2;
                    *dyn_data = DIGITS[high];
                    _delay_us(150);
                    *dyn_data = 0;

                    // LOW
                    *dyn_place = 3;
                    *dyn_data = DIGITS[low];
                    _delay_us(150);
                    *dyn_data = 0;
                }
            }
        }
    }
}

