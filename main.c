/* 
 * File:   main.c
 * Author: Dylan
 *
 * Created on April 24, 2019, 7:46 PM
 */


#include "xc.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "stdbool.h"
#include "seifo002_adcLib.h"
#include "mtdm_final_LEDMat_Clib.h"
#include "seifo002_stickLib.h"
#include "drais007_LCD2_CLib_v001.h"
#include "mazeGame.h"
#include "snakeGame_v002.h"

// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled. 
					// Primary Oscillator refers to an external osc connected to the OSC1 and OSC2 pins)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // OSC2/CLKO/RC15 functions as port I/O (RC15)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


#define numGames 2

volatile int game = 2;
volatile bool run;

//Add a button interrupt here that increments the game variable and sets run to false

//    game = (game+1)%numGames;
//    run = false;

void setup(void) {
    CLKDIVbits.RCDIV = 0;

    //Configure IO
    AD1PCFG = 0x9fff;           //sets all pins to digital I/O
    TRISA = 0xffff;             //Set port A to inputs
    TRISB = 0xffff;             //Set port B to inputs
    LATA = 0xffff;              //Set all of port A to HIGH
    LATB = 0xffff;              //Set all of port B to HIGH

    //Setup RB15 as output
    TRISBbits.TRISB15 = 0;      //Set RB15 output (heartbeat LED)
    
    // Setup Timer4 to 10 ms delay
    T4CON = 0;
    T4CONbits.TCKPS = 0b01; //1:8 prescalar
    PR4 = 19999;
    TMR4 = 0;
    IFS1bits.T4IF = 0;
    T4CONbits.TON = 1;
}

void wait() {
    while (!IFS1bits.T4IF);
    IFS1bits.T4IF = 0;
}

int main(void) {
    setup();
    
    adcSetup();

    lcd_init(0b100000);
    
    LEDMatInit();
    
    initMazeGame();
    initSnakeGame();
    
    while(1) {
        //runMazeGame();
        runSnakeGame();
    }
    
    return 0;
}