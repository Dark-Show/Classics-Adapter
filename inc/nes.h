
#ifndef _NES_H_
#define _NES_H_



/**********************************************************
 * nes button masks and defines
 */

uint8_t BUTTON = 0;
uint8_t DPAD = 0;
uint8_t VIRTUAL = 0;

#define BUTTON_A        (1 << 0)
#define BUTTON_B        (1 << 1)
#define BUTTON_SELECT   (1 << 2)
#define BUTTON_START    (1 << 3)

#define DPAD_UP         (1 << 0)
#define DPAD_DOWN       (1 << 1)
#define DPAD_LEFT       (1 << 2)
#define DPAD_RIGHT      (1 << 3)

#define VIRTUAL_REBOOT  (1 << 1)

/**********************************************************
 * gamepad button defines
 */

#define GAMEPAD_CIRCLE_ON	    (BUTTON & BUTTON_A)
#define GAMEPAD_CROSS_ON	    (BUTTON & BUTTON_B)
#define GAMEPAD_START_ON		(BUTTON & BUTTON_START)
#define GAMEPAD_SELECT_ON	    (BUTTON & BUTTON_SELECT)

#define GAMEPAD_UP_ON	        (DPAD & DPAD_UP)
#define GAMEPAD_DOWN_ON	      (DPAD & DPAD_DOWN)
#define GAMEPAD_LEFT_ON       (DPAD & DPAD_LEFT)
#define GAMEPAD_RIGHT_ON	    (DPAD & DPAD_RIGHT)

#define GAMEPAD_REBOOT_ON     (VIRTUAL & VIRTUAL_REBOOT)

#define GAMEPAD_TRIANGLE_ON     0
#define GAMEPAD_SQUARE_ON       0
#define GAMEPAD_L1_ON           0
#define GAMEPAD_R1_ON           0
#define GAMEPAD_L2_ON           0
#define GAMEPAD_R2_ON           0

/**********************************************************
 * nes pinout
 *
 * VCC (white)    --->    VCC
 * Clock (red) --->    PD0
 * Latch (orange) --->    PD1
 * Data (yellow)     --->    PD2
 * GND (brown)    --->    GND
 */

#define CLOCK   (1 << 0)
#define LATCH   (1 << 1)
#define DATA    (1 << 2)

/**********************************************************
 * initialize pins
 */

void gamepad_init(void) {
    DDRD |= CLOCK;  // output
    DDRD |= LATCH;  // output
    DDRD &= ~DATA;  // input

    PORTD |= CLOCK; // high output
    PORTD |= LATCH; // high output
    PORTD &= ~DATA; // normal input
}

/**********************************************************
 * read pins and set buttons masks
 */

void gamepad_read(void) {
    // set latch low
    PORTD &= ~LATCH;

    // read data pin, low is a press                cycle clock for next button
                                                    PORTD &= ~CLOCK; PORTD |= CLOCK;
    BUTTON  = (PIND & DATA) ? 0 : BUTTON_A;         PORTD &= ~CLOCK; PORTD |= CLOCK;
    BUTTON |= (PIND & DATA) ? 0 : BUTTON_B;         PORTD &= ~CLOCK; PORTD |= CLOCK;
    BUTTON |= (PIND & DATA) ? 0 : BUTTON_SELECT;    PORTD &= ~CLOCK; PORTD |= CLOCK;
    BUTTON |= (PIND & DATA) ? 0 : BUTTON_START;     PORTD &= ~CLOCK; PORTD |= CLOCK;
    DPAD    = (PIND & DATA) ? 0 : DPAD_UP;          PORTD &= ~CLOCK; PORTD |= CLOCK;
    DPAD   |= (PIND & DATA) ? 0 : DPAD_DOWN;        PORTD &= ~CLOCK; PORTD |= CLOCK;
    DPAD   |= (PIND & DATA) ? 0 : DPAD_LEFT;        PORTD &= ~CLOCK; PORTD |= CLOCK;
    DPAD   |= (PIND & DATA) ? 0 : DPAD_RIGHT;       // PORTD &= ~CLOCK; PORTD |= CLOCK;

    // set latch high
    PORTD |= LATCH;

    // handle virtual buttons
    // reboot   : select + start + A + B
	
    VIRTUAL = 0;	
	
	// if all buttons pressed
    if ((BUTTON & 0x0F) == 0x0F) {
      // reboot
      VIRTUAL = VIRTUAL_REBOOT;
      // eat buttons
      BUTTON = 0;
    }
}

#endif
