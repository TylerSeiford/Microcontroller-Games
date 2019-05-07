/* 
 * File:   mtdm_final_LEDMat_Clib.h
 * Author: Michael
 *
 * Created on April 18, 2019, 11:32 AM
 */

#ifndef MTDM_FINAL_LEDMAT_CLIB_H
#define	MTDM_FINAL_LEDMAT_CLIB_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif

void delay(int ms);
void LEDMatDispTest(char x, char y);
void LEDMatWriteScreen();
void LEDMatCmd(char com);
void LEDMatInit();
void LEDMatUpdateBuf(int* screenArr);
void LEDMatTurnOn(int x, int y);
void LEDMatTurnOff(int x, int y);
void LEDMatClearScreen();

#ifdef	__cplusplus
}
#endif

#endif	/* MTDM_FINAL_LEDMAT_CLIB_H */

