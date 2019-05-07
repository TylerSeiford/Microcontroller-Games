/*
 * File:   seifo002_ADC_lib.c
 * Author: Tyler
 *
 * Created on April 22, 2019, 11:17 AM
 */
#include "xc.h"

volatile int vert, horiz;

void adcSetup() {
    //Configure TMR3 for 16 times a second
    T3CON = 0;                      //Clear register
    T3CONbits.TCKPS = 0b10;         //Set prescalar to 1:64
    PR3 = 3900;                     //Set preset to 3900 (roughly 64 times per second)
    IEC0bits.T3IE = 0b0;            //Disable interrupt

    TRISAbits.TRISA0 = 0b1;         //Configure AN0/RA0 as input
    AD1PCFGbits.PCFG0 = 0b0;        //Configure AN0 as analog
    TRISAbits.TRISA1 = 0b1;         //Configure AN1/RA1 as input
    AD1PCFGbits.PCFG1 = 0b0;        //Configure AN1 as analog
    
    AD1CSSL = 0x0003;               //Configure to scan AN0 & AN1
    AD1CON2bits.CSCNA = 0b1;        //Configure to scan mode on MUX1

    AD1CON2bits.SMPI = 0b0001;      //Configure to interrupt every other conversion
    AD1CON1bits.SSRC = 0b010;       //Setup trigger source bits (TMR3)
    AD1CON1bits.FORM = 0b00;        //Setup format of output (integer)
    
    AD1CHS = 0x0000;                //Clear other control registers
    AD1CON3 = 0x0000;
    
    IEC0bits.AD1IE = 0b1;           //Enable interrupts on ADC
    T3CONbits.TON = 1;              //Enable TMR3
    AD1CON1bits.ADON = 0b1;         //Enable ADC1
    AD1CON1bits.ASAM = 0b1;         //Setup start of sampling (Auto)
}

void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt() {
    vert = ADC1BUF0;
    horiz = ADC1BUF1;
    IFS0bits.AD1IF = 0;
}

/*
 * Returns the unsigned 10 bit value of the ADC
 */
uint16_t adcGetRawX() {
	return horiz;
}

/*
 * Returns the unsigned 10 bit value of the ADC
 */
uint16_t adcGetRawY() {
	return vert;
}