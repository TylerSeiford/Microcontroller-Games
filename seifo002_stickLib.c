/*
 * File:   seifo002_joystickLib.c
 * Author: Tyler
 *
 * Created on April 24, 2019, 10:56 AM
 */
#include "xc.h"
#include "seifo002_adcLib.h"

#define DEADBAND 96

uint16_t stickState, stickLast;

/*
 * Sets up Joystick button on RB15
 */
void stickSetup() {
    TRISBbits.TRISB15 = 0b1;    //Setup RB15 as input
    CNPU1bits.CN11PUE = 0b1;    //Setup RB15/CN11 pull up resistor
}

/*
 * Returns a signed 10 bit value of the ADC.  0 is "center", -512 is "left", and 512 is "right".
 */
int16_t stickGetX() {
	return (((int32_t)adcGetRawX()) - 486) * -1;
}

/*
 * Returns a signed 10 bit value of the ADC.  0 is "center", -512 is "down", and 512 is "up"
 */
int16_t stickGetY() {
	return ((int32_t)adcGetRawY()) - 510;
	
}

/*
 *  0x00: Centered
 *  0x01: Moving up
 *  0x02: Moving right
 *  0x04: Moving down
 *  0x08: Moving left
 *  0x10: Pressed
 */
uint16_t stickGetState() {
	return stickState;
}

/*
 *  0x0: Not centered
 *  0x1: Centered
 */
uint8_t stickCentered() {
	return stickState == 0;
}

/*
 *  0x0: Not moving up
 *  0x1: Moving up
 */
uint8_t stickUp() {
	return (stickState & 0x01);
}

/*
 *  0x0: Not moving right
 *  0x2: Moving right
 */
uint8_t stickRight() {
	return (stickState & 0x02);
}

/*
 *  0x0: Not moving down
 *  0x4: Moving down
 */
uint8_t stickDown() {
	return (stickState & 0x04);
}

/*
 *  0x0: Not moving left
 *  0x8: Moving left
 */
uint8_t stickLeft() {
	return (stickState & 0x08);
}

/*
 *  0x0: Not pressed
 *  0x1: Pressed
 */
uint8_t stickPressed() {
	return (stickState>>4 & 0x01);
}

/*
 * Updates stickState from the ADC buffer and button input
 */
void stickUpdate() {
	int16_t x = stickGetX(), y = stickGetY();
    stickLast = stickState;
    
    if(y > DEADBAND) {          //Upwards
        if(x > DEADBAND) {          //Also Right
            if(y > x) {                 //Up
                stickState = 0x01;
                return;
            }
            else {                      //Right
                stickState = 0x02;
                return;
            }
        }
        else if(x < -DEADBAND) {    //Also Left
            if(y > -x) {                //Up
                stickState = 0x01;
                return;
            }
            else {                      //Left
                stickState = 0x08;
                return;
            }
        }
        else {                      //Just Up
            stickState = 0x01;
            return;
        }
    }
    else if(y < -DEADBAND) {    //Downwards
        if(x > DEADBAND) {          //Also Right
            if(-y > x) {                //Down
                stickState = 0x04;
                return;
            }
            else {                      //Right
                stickState = 0x02;
                return;
            }
        }
        else if(x < -DEADBAND) {    //Also Left
            if(-y > x) {                //Down
                stickState = 0x04;
                return;
            }
            else {                      //Left
                stickState = 0x08;
                return;
            }
        }
        else {                      //Just Down
            stickState = 0x04;
            return;
        }
    }
    else if(x > DEADBAND) {     //Just Right
        stickState = 0x02;    
        return;
    }
    else if(x < -DEADBAND) {    //Just Left
        stickState = 0x08;
        return;
    }
    else {                      //Centered
        stickState = 0x00;
    }
    
    if(PORTBbits.RB15 == 0) {   //Button pressed
        stickState = 0x10;
    }
}

/*
 * Returns what has changed since the last state update
 * Examples:
 *   0x01->0x11: 0x10
 *   0x01->0x08: 0x09
 *   0x01->0x01: 0x00
 */
uint16_t stickChanged() {
    return stickState ^ stickLast;
}