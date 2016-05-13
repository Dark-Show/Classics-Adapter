
#ifndef _GENS_H_
#define _GENS_H_

/**********************************************************
 * genesis button masks and defines
 */

uint8_t MODE = 0;   //Used for MODE button detection.
uint8_t BTN6 = 0;   //Default 3 button mode.
uint8_t BUTTON = 0;
uint8_t DPAD = 0;
// uint8_t VIRTUAL = 0;


// BUG: If bit 0 is used, state is always 1.
#define BUTTON_A        (1 << 1)
#define BUTTON_B        (1 << 2)
#define BUTTON_C        (1 << 3)
#define BUTTON_X        (1 << 4)
#define BUTTON_Y        (1 << 5)
#define BUTTON_Z        (1 << 6)
#define BUTTON_START    (1 << 7)

#define BUTTON_MODE     (1 << 1)

#define DPAD_UP         (1 << 1)
#define DPAD_DOWN       (1 << 2)
#define DPAD_LEFT       (1 << 3)
#define DPAD_RIGHT      (1 << 4)

/**********************************************************
 * gamepad button defines
 */

#define GAMEPAD_CIRCLE_ON	    (BUTTON & BUTTON_C)
#define GAMEPAD_CROSS_ON	    (BUTTON & BUTTON_B)
#define GAMEPAD_SQUARE_ON       (BUTTON & BUTTON_A)
#define GAMEPAD_START_ON		(BUTTON & BUTTON_START)

#define GAMEPAD_UP_ON	        (DPAD & DPAD_UP)
#define GAMEPAD_DOWN_ON	        (DPAD & DPAD_DOWN)
#define GAMEPAD_LEFT_ON         (DPAD & DPAD_LEFT)
#define GAMEPAD_RIGHT_ON	    (DPAD & DPAD_RIGHT)

#define GAMEPAD_SELECT_ON	    (MODE & BUTTON_MODE)
#define GAMEPAD_TRIANGLE_ON     (BUTTON & BUTTON_X)
#define GAMEPAD_L1_ON           (BUTTON & BUTTON_Y)
#define GAMEPAD_R1_ON           (BUTTON & BUTTON_Z)
#define GAMEPAD_L2_ON           0
#define GAMEPAD_R2_ON           0
#define GAMEPAD_REBOOT_ON       0

/**********************************************************
 * Genesis pinout
 * pin |!select  |select  |6btn 3rd pulse
 * 1   |UP       |UP      |Z
 * 2   |DOWN     |DOWN    |Y
 * 3   |GROUND   |LEFT    |X
 * 4   |GROUND   |RIGHT   |MODE
 * 5   |+5V      |        |
 * 6   |A        |B       |
 * 7   |SELECT   |        |
 * 8   |GROUND   |        |
 * 9   |START    |C       |
 */
 
// Port B
#define SELECT      (1 << 0)    // Pin 7
#define UP_Z        (1 << 1)    // Pin 1
#define DOWN_Y      (1 << 2)    // Pin 2
#define LEFT_X      (1 << 3)    // Pin 3
#define RIGHT_MODE  (1 << 7)    // Pin 4

// Port D
#define A_B         (1 << 0)    // Pin 6
#define START_C     (1 << 1)    // Pin 9

/**********************************************************
 * initialize pins
 */ 

void gamepad_init(void) {
    //DDRD |= (1 << 6); // LED (T2.0)
    
    DDRB |= SELECT;      // output
    DDRB &= ~UP_Z;       // input
    DDRB &= ~DOWN_Y;     // input
    DDRB &= ~LEFT_X;     // input
    DDRB &= ~RIGHT_MODE; // input
    DDRD &= ~A_B;        // input
    DDRD &= ~START_C;    // input

    PORTB |= SELECT;     // high output
    PORTB &= ~UP_Z;      // notmal input
    PORTB &= ~DOWN_Y;    // notmal input
    PORTB &= ~LEFT_X;    // notmal input
    PORTB &= ~RIGHT_MODE;// normal input
    PORTD &= ~A_B;       // notmal input
    PORTD &= ~START_C;   // normal input
    
}

/**********************************************************
 * read pins and set buttons masks
 */ 

void gamepad_read(void) {
    PORTB  |= SELECT; //CYCLE 1
    _delay_ms(1);
    DPAD   = (PINB & UP_Z) ? 1 : DPAD_UP;
    DPAD   |= (PINB & DOWN_Y) ? 1 : DPAD_DOWN;
    DPAD   |= (PINB & LEFT_X) ? 1 : DPAD_LEFT;
    DPAD   |= (PINB & RIGHT_MODE) ? 1 : DPAD_RIGHT;
    BUTTON = (PIND & A_B) ? 1 : BUTTON_B;
    BUTTON |= (PIND & START_C) ? 1 : BUTTON_C;
    
    PORTB  &= ~SELECT; //CYCLE 2
    _delay_ms(1);
    BUTTON |= (PIND & START_C) ? 1 : BUTTON_START;
    BUTTON |= (PIND & A_B) ? 1 : BUTTON_A;
    
    
    PORTB  |= SELECT; PORTB  &= ~SELECT; //CYCLE 3-4
    PORTB  |= SELECT; PORTB  &= ~SELECT; //CYCLE 5-6
    
    /**********************************************************
     * 6 Button controller auto-detection.
     * On cycle 6, check if all directional buttons are off.
     * If so set BTN6. If at any time this changes (on cycle 6)
     * unset BTN6.
     */
    
    if(~(PINB & UP_Z)){
        if(~(PINB & DOWN_Y)){
            if(~(PINB & LEFT_X)){
                if(~(PINB & RIGHT_MODE)){
                    BTN6=1;
                }
            }
        }
    }
    
    if(PINB & UP_Z){BTN6=0;}
    if(PINB & DOWN_Y){BTN6=0;}
    if(PINB & LEFT_X){BTN6=0;}
    if(PINB & RIGHT_MODE){BTN6=0;}
    
    if(BTN6){
        PORTB  |= SELECT; //CYCLE 7
        _delay_ms(1);
        BUTTON |= (PINB & UP_Z) ? 1 : BUTTON_Z;
        BUTTON |= (PINB & DOWN_Y) ? 1 : BUTTON_Y;
        BUTTON |= (PINB & LEFT_X) ? 1 : BUTTON_X;
        MODE = (PINB & RIGHT_MODE) ? 1 : BUTTON_MODE;
        PORTB  &= ~SELECT; //CYCLE 8
    }else{
        PORTB  |= SELECT; //CYCLE 7
        PORTB  &= ~SELECT; //CYCLE 8
        if(PINB & UP_Z){
            if(PINB & DOWN_Y){
                BTN6=1;
            }
        }
    }
}

#endif



