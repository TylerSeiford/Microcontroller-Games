/* 
 * File:   seifo002_joystickLib.h
 * Author: Tyler
 *
 * Created on April 24, 2019, 10:55 AM
 */

#ifndef SEIFO002_JOYSTICKLIB_H
#define	SEIFO002_JOYSTICKLIB_H

#ifdef	__cplusplus
extern "C" {
#endif

    
/*
 * Sets up Joystick button on RB15
 */
void stickSetup();

/*
 * Updates state from the ADC buffers
 */
void stickUpdate();

/*
 * Returns what has changed since the last state update
 * Examples:
 *   0x01->0x11: 0x10
 *   0x01->0x08: 0x09
 *   0x01->0x01: 0x00
 */
uint16_t stickChanged();

/*
 *  0x0: Not centered
 *  0x1: Centered
 */
uint8_t stickCentered();

/*
 *  0x0: Not moving up
 *  0x1: Moving up
 */
uint8_t stickUp();

/*
 *  0x0: Not moving right
 *  0x2: Moving right
 */
uint8_t stickRight();

/*
 *  0x0: Not moving down
 *  0x4: Moving down
 */
uint8_t stickDown();

/*
 *  0x0: Not moving left
 *  0x8: Moving left
 */
uint8_t stickLeft();

/*
 *  0x0: Not pressed
 *  0x1: Pressed
 */
uint8_t stickPressed();


/* BELOW FUNCTIONS ARE FOR INTERNAL USAGE OR FOR THOSE WHO KNOW WHAT THEY ARE DOING */


/*
 *  0x00: Centered
 *  0x01: Moving up
 *  0x02: Moving right
 *  0x04: Moving down
 *  0x08: Moving left
 *  0x10: Pressed
 */
uint16_t stickGetState();

/*
 * Returns a signed 10 bit value of the ADC.  0 is "center", -512 is "left", and 512 is "right".
 */
int16_t stickGetX();

/*
 * Returns a signed 10 bit value of the ADC.  0 is "center", -512 is "down", and 512 is "up"
 */
int16_t stickGetY();


#ifdef	__cplusplus
}
#endif

#endif	/* SEIFO002_JOYSTICKLIB_H */

