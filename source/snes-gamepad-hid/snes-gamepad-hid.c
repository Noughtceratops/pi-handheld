#include <avr/io.h>
#include <util/delay.h>
#include "usb_gamepad.h"

#define LED_CONFIG (DDRD |= (1 << 6))
#define LED_OFF (PORTD &= ~(1 << 6))
#define LED_ON (PORTD |= (1 << 6))

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

// Port B
#define CLOCK_PIN 0
#define LATCH_PIN 1
#define DATA_PIN 2

#define SAMPLE_FRAMES_PER_SECOND 30
#define PULSES_PER_SECOND (16 * SAMPLE_FRAMES_PER_SECOND)

int main (void) {
    CPU_PRESCALE(0);
    LED_CONFIG;
    LED_ON;

    DDRB = (1 << CLOCK_PIN) | (1 << LATCH_PIN);
    PORTB = 0; // (1 << CLOCK_PIN);

    usb_init();
    while (!usb_configured()) {
        continue;
    }

    _delay_ms(1000);
    LED_OFF;
    int light = 1;
    while (1) {
	// Pulse the latch to load up the shift register
	PORTB |= (1 << LATCH_PIN);
        _delay_us(12);
	PORTB &= ~(1 << LATCH_PIN);

	uint16_t bits = 0;
	if ((PINB & (1 << DATA_PIN)) == 0) {
            bits |= 1;
	}
        _delay_us(6);

        for (int bit = 1; bit < 16; ++bit) {    
	    PORTB |= (1 << CLOCK_PIN);
	    _delay_us(6);
	    if ((PINB & (1 << DATA_PIN)) == 0) {
                bits |= 1 << bit;
	    }
	    PORTB &= ~(1 << CLOCK_PIN);
	    _delay_us(6);
	};
	
        usb_gamepad_action(bits);
	_delay_ms(16);
	light = !light;
	if (bits) {
            LED_ON;
        }
        else {
            LED_OFF;
        }
    }
}
