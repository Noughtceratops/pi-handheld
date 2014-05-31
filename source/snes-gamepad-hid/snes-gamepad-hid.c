#include <avr/io.h>
#include <util/delay.h>
#include "usb_gamepad.h"

#define LED_CONFIG (DDRD |= (1 << 6))
#define LED_OFF (PORTD &= ~(1 << 6))
#define LED_ON (PORTD |= (1 << 6))

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

// Port B
#define DPAD_UP (1 << 0)
#define DPAD_DOWN (1 << 1)
#define DPAD_LEFT (1 << 2)
#define DPAD_RIGHT (1 << 3)
#define BUTTON_A (1 << 4)
#define BUTTON_B (1 << 5)
#define BUTTON_X (1 << 6)
#define BUTTON_Y (1 << 7)

#define PORT_B_PINS (DPAD_UP | DPAD_DOWN | DPAD_LEFT | DPAD_RIGHT | BUTTON_A | BUTTON_B | BUTTON_X | BUTTON_Y)

// Port D
#define BUTTON_L (1 << 0)
#define BUTTON_R (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START (1 << 3) 

#define PORT_D_PINS (BUTTON_L | BUTTON_R | BUTTON_SELECT | BUTTON_START)

int main (void) {
    CPU_PRESCALE(0);
    LED_CONFIG;
    LED_ON;

    // Configure all of our pins as inputs with pullup resistors
    DDRD = 0x00;
    DDRB = 0x00;
    PORTB = PORT_B_PINS;
    PORTD = PORT_D_PINS;

    usb_init();
    while (!usb_configured()) {
        continue;
    }

    _delay_ms(1000);
    LED_OFF;
    while (1) {
        uint8_t b = ~PINB & PORT_B_PINS;
        uint8_t d = ~PIND & PORT_D_PINS;
        if (b || d) {
            LED_ON;
        }
        else {
            LED_OFF;
        }
    }
}
