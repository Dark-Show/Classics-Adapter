
#ifndef _NES_H_
#define _NES_H_



/**********************************************************
 * nes button masks and defines
 */

uint8_t BUTTON = 0;
uint8_t DPAD = 0;

#define BUTTON_A        (1 << 0)
#define BUTTON_B        (1 << 1)
#define BUTTON_SELECT   (1 << 2)
#define BUTTON_START    (1 << 3)

#define DPAD_UP         (1 << 0)
#define DPAD_DOWN       (1 << 1)
#define DPAD_LEFT       (1 << 2)
#define DPAD_RIGHT      (1 << 3)

/**********************************************************
 * gamepad button defines
 */

#define GAMEPAD_CIRCLE_ON	    (BUTTON & BUTTON_A)
#define GAMEPAD_CROSS_ON	    (BUTTON & BUTTON_B)
#define GAMEPAD_START_ON		(BUTTON & BUTTON_START)
#define GAMEPAD_SELECT_ON	    (BUTTON & BUTTON_SELECT)

#define GAMEPAD_UP_ON	        (DPAD & DPAD_UP)
#define GAMEPAD_DOWN_ON	        (DPAD & DPAD_DOWN)
#define GAMEPAD_LEFT_ON         (DPAD & DPAD_LEFT)
#define GAMEPAD_RIGHT_ON	    (DPAD & DPAD_RIGHT)


#define GAMEPAD_TRIANGLE_ON     0
#define GAMEPAD_SQUARE_ON       0
#define GAMEPAD_L1_ON           0
#define GAMEPAD_R1_ON           0
#define GAMEPAD_L2_ON           0
#define GAMEPAD_R2_ON           0
#define GAMEPAD_REBOOT_ON       0

/**********************************************************
 * nes pinout
 *
 * VCC (white)    --->    VCC
 * Clock (red) --->    PB0
 * Latch (orange) --->    PB1
 * Data (yellow)     --->    PB2
 * GND (brown)    --->    GND
 */

#define CLOCK   (1 << 0)
#define LATCH   (1 << 1)
#define DATA    (1 << 2)

/**********************************************************
 * initialize pins
 */ 

void gamepad_init(void) {
    DDRB |= CLOCK;  // output
    DDRB |= LATCH;  // output
    DDRB &= ~DATA;  // input

    PORTB |= CLOCK; // high output
    PORTB |= LATCH; // high output
    PORTB &= ~DATA; // normal input
}

/**********************************************************
 * read pins and set buttons masks
 */ 

void gamepad_read(void) {
    // set latch low
    PORTB &= ~LATCH;

    // read data pin, low is a press                cycle clock for next button
                                                    PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON  = (PINB & DATA) ? 0 : BUTTON_A;         PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_B;         PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_SELECT;    PORTB &= ~CLOCK; PORTB |= CLOCK;
    BUTTON |= (PINB & DATA) ? 0 : BUTTON_START;     PORTB &= ~CLOCK; PORTB |= CLOCK;
    DPAD    = (PINB & DATA) ? 0 : DPAD_UP;          PORTB &= ~CLOCK; PORTB |= CLOCK;
    DPAD   |= (PINB & DATA) ? 0 : DPAD_DOWN;        PORTB &= ~CLOCK; PORTB |= CLOCK;
    DPAD   |= (PINB & DATA) ? 0 : DPAD_LEFT;        PORTB &= ~CLOCK; PORTB |= CLOCK;
    DPAD   |= (PINB & DATA) ? 0 : DPAD_RIGHT;       // PORTB &= ~CLOCK; PORTB |= CLOCK;

    // set latch high
    PORTB |= LATCH;
}

#endif

