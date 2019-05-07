/* 
 * File:   drais007_LCD2_CLib_v001.h
 * Author: Michael
 *
 * Created on April 5, 2019, 2:01 PM
 */

#ifndef DRAIS007_LCD2_CLIB_V001_H
#define	DRAIS007_LCD2_CLIB_V001_H

#include <xc.h> // include processor files - each processor file is guarded. 

#ifdef	__cplusplus
extern "C" {
#endif


 // Insert declarations
    void lcd_cmd(char data);
    void delay(int ms);
    void lcd_init(char contrast);
    void lcd_setCursor(short x, short y);
    void lcd_printChar(char c);
    void lcd_printString(const char* string, int length);
    void lcd_shiftScreen();

#ifdef	__cplusplus
}
#endif

#endif	/* DRAIS007_LCD2_CLIB_V001_H */

