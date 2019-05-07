/* 
 * File:   seifo002_ADC_lib.h
 * Author: Tyler
 *
 * Created on April 22, 2019, 11:17 AM
 */

#ifndef SEIFO002_ADC_LIB_H
#define	SEIFO002_ADC_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif


/*
 * Sets up ADC for scan mode on MUX1, with AN0 (Vertical) & AN1 (Horizantal)
 * ADC is setup to interrupt 64 times a second and update the library's buffers for X & Y values.
 */
void adcSetup();

/*
 * Returns the unsigned 10 bit value of the X buffer
 */
uint16_t adcGetRawX();

/*
 * Returns the unsigned 10 bit value of the Y buffer
 */
uint16_t adcGetRawY();


#ifdef	__cplusplus
}
#endif

#endif	/* SEIFO002_ADC_LIB_H */

