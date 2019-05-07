
// LCD Library
//    runs the LCD screen 
// by: Michael Drais
// date: Apr 5, 2019
//

#include "xc.h"
#include "drais007_LCD2_CLib_v001.h"

#define LCDADRESS 0b01111100;

void lcd_cmd(char data){
   
    I2C2CONbits.SEN = 1;	//Initiate Start condition
    while(I2C2CONbits.SEN ==1); // SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = LCDADRESS; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
   //   LATB &= 0xFFBF;
    IFS3bits.MI2C2IF = 0;
    //  delay(1);
    
    I2C2TRN = 0b00000000; // 8-bits consisting of control byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
     // delay(1);
  
    I2C2TRN = data; // 8-bits consisting of the data byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN ==1); // PEN will clear when Stop bit is complete
}


void delayLCD(int ms){
    while(ms > 0){
        while(!IFS0bits.T1IF);
        _T1IF = 0;
        ms --;
    }
}



void lcd_init(char contrast){
    char contrastCmd1 = 0b01110000;
    char contrastCmd2 = 0b01010100;
    contrastCmd1 |= (contrast & 0b1111);
    char contrastFirst2 = (contrast & 0b110000) >> 4;
    contrastCmd2 |= contrastFirst2;
    
    LATB &= 0xFFBF;
    delay(1);
    LATB |= 0b1000000;
    
    delay(100);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(contrastCmd1); // contrast C3-C0
    lcd_cmd(contrastCmd2); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    delay(201);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    delay(3);

}

void lcd_setCursor(short x, short y){
    char pos = (0x40 * x) + y;
    pos |= 10000000;
    lcd_cmd(pos);
}
void lcd_printChar(char c){
    
    I2C2CONbits.SEN = 1;	//Initiate Start condition
    while(I2C2CONbits.SEN ==1); // SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = LCDADRESS; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
     // delay(1);
    
    I2C2TRN = 0b11000000; // 8-bits consisting of control byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
     // delay(1);
    
    I2C2TRN = c; // 8-bits consisting of the data byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
     // delay(1);
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN ==1); // PEN will clear when Stop bit is complete
}

void lcd_printString(const char* string, int length){
    //int length = sizeof(string);
    int i =0;
    for(i = 0; i < length; i ++){
        lcd_printChar(string[i]);
    }
}

void lcd_shiftScreen(){
    lcd_cmd(0b00011000);
}
