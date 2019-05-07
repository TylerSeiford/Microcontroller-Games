// LED Matrix Library
//    can write the screen to be whatever is input
// by: Michael Drais
// email: drais007@umn.edu
// date: Apr 18, 2019

#include "xc.h"
#include "mtdm_final_LEDMat_Clib.h"

unsigned int screenBuf[8];

void delay(int ms){
    //delays for ms miliseconds, using T1
    while(ms > 0){
        while(!IFS0bits.T1IF);
        _T1IF = 0;
        ms --;
    }
}
void LEDMatDispTest(char x, char y){
    //Test pushing stuff to the screen, used in the preliminary steps
    // TESTING ONLY DO NOT USE
    I2C2CONbits.SEN = 1;	//Initiate Start condition
    while(I2C2CONbits.SEN ==1); // SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b11100000; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    
    I2C2TRN = 0b00000000; // set cursor to 00
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = x; // write first row
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b00000000; // writes to unused memory area, advances cursor to next row 
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = y; // write a second row
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b00000000; // again, move cursor
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN ==1); // PEN will clear when Stop Bit is complete
}
void LEDMatWriteScreen(){
    //writes the current internal buffer, screenBuf to the screen
    I2C2CONbits.SEN = 1;	//Initiate Start condition
    while(I2C2CONbits.SEN ==1); // SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b11100000; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    
    I2C2TRN = 0b00000000; // reset cursor to 0,0
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    int i =0;
    for(i =0; i < 8; i ++){ //cycle through buffer and write all rows
        I2C2TRN = (screenBuf[i] & 0xFF); // write one row
        while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
    
        I2C2TRN = 0b00000000; // write bogus to unused memory space to advance cursor
         while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
    }
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN ==1); // PEN will clear when Stop Bit is complete
}
void LEDMatCmd(char com){
    //used to only send a command byte with out data, used during the setup
    
    I2C2CONbits.SEN = 1;	//Initiate Start condition
    while(I2C2CONbits.SEN ==1); // SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b11100000; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    
    I2C2TRN = com; // send the command packet
    while(IFS3bits.MI2C2IF == 0);
 
   // IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN ==1); // PEN will clear when Stop Bit is complete
}
void LEDMatInit(){
    //inits all necissary parts of the code
    CLKDIVbits.RCDIV = 0;
    TRISB &= 0xFFFB;
    
    //Sets up Ti for a 1 ms delay 
    T1CON = 0;
    PR1 = 15999;
    TMR1 = 0;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE =0;
    T1CONbits.TON = 1;
    
    //sends the reset signal
    LATB &= 0xFFFB;
    delay(40);
    
    //sets up the I2C
    I2C2CONbits.I2CEN =0;
    I2C2BRG = 157;
    //_MI2C2IE = 1;
    _MI2C2IF = 0;
    _I2CSIDL =0;
    I2C2CONbits.I2CEN =1;
    
    //clears the screen buffer
    int i =0;
    for(i =0; i < 8; i ++){
        screenBuf[i] = 0;
    }
    /* testing code, manually write to screen Buffer
    screenBuf[0]= 0b00000000;
    screenBuf[1]= 0b11111111;
    screenBuf[2]= 0b11111111;
    screenBuf[3]= 0b01001100;        
    screenBuf[4]= 0b01001110;
    screenBuf[5]= 0b01001011;
    screenBuf[6]= 0b11111001;
    screenBuf[7]= 0b10110000;  
    */
    
    //sends setup command
    LEDMatCmd(0b00100001);//turns on Oscilator
    LEDMatCmd(0b10100000);//sets devise for ROW commands 
    LEDMatCmd(0b11100011);//sets brightness
    LEDMatCmd(0b10000001);//turns on the screen and disables blinking


}
void LEDMatUpdateBuf(int* screenArr){
    //send what the screen looks like and writes to the buffer
    //NOTE: there is a wired scroll that the device does, this function handles this
    int i =0;
    //int j =0;
    short temp =0;
    for( i =0; i < 8; i ++){
        //for(j = 0; j <8; j ++){
         temp = screenArr[i] >> 1;// these couple lines handles the odd scroll the Matrix does 
            
        if((screenArr[i] & 1) != 0){
            temp |= 0b10000000;   
        }
            
        //}
        screenBuf[7-i] = temp;
    }
}
void LEDMatTurnOn(int x, int y){
    if(x>7){x=7;}
    if(x<0){x=0;}
    if(y>7){y=7;}
    if(y<0){y=0;}
    if(x == 0){
        x= 8;
    }
    screenBuf[7-y] |= (1<<(x-1));
}
void LEDMatTurnOff(int x, int y){
    if(x>7){x=7;}
    if(x<0){x=0;}
    if(y>7){y=7;}
    if(y<0){y=0;}
    int mask = 0b11111111;
    if(x == 0){
        x= 8;
    }
    mask ^= (1 << (x-1));
    screenBuf[7-y] &= mask;
}

void LEDMatClearScreen(){
    int i;
    for(i = 0; i < 8; i ++){
        screenBuf[i] = 0;
    }
}

void LEDMatSetBrightness(int brightness){
    int bright = 0;
    bright = brightness;
    int cmd = 0b11100000;
    if(bright < 0){
        bright = 0;
    }
    if(bright > 15){
        bright = 15;
    }
    cmd |= bright;
    LEDMatCmd(cmd);//sets brightness
}

