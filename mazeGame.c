#include <xc.h>
#include "stdlib.h"
#include "stdio.h"
#include "drais007_LCD2_CLib_v001.h"
#include "mtdm_final_LEDMat_Clib.h"
#include "seifo002_stickLib.h"
uint8_t diff = 0;
uint8_t lastDoor = 0;
uint8_t lastRoom = 255;

uint8_t x;
uint8_t y;
uint8_t flash;

int *room;
int w[8] = {0b0,0b10000010,0b10010010,0b10101010,0b10101010,0b01000100,0b01000100,0b0};
int l[8] = {0b0,0b00010000,0b00010000,0b00010000,0b00010000,0b00011100, 0b0, 0b0};
int start[8] = {0b11101000, 0b11111011, 0b11110011, 0b01110110, 0b11110111, 0b11111111, 0b11111111, 0b11101111};

uint8_t isItAWall(uint8_t x, uint8_t y) {
    if(x == 0)
    {
        return (0b00000001 & room[y]) > 0;
    }
    else if(x == 1)
    {
        return (0b00000010 & room[y]) > 0;
    }
    else if(x == 2)
    {
        return (0b00000100 & room[y]) > 0;
    }
    else if(x == 3)
    {
        return (0b00001000 & room[y]) > 0;
    }
    else if(x == 4)
    {
        return (0b00010000 & room[y]) > 0;
    }
    else if(x == 5)
    {
        return (0b00100000 & room[y]) > 0;
    }
    else if(x == 6)
    {
        return (0b01000000 & room[y]) > 0;
    }
    else if(x == 7)
    {
        return (0b10000000 & room[y]) > 0;
    }
    else {
        return 1;
    }
}

int* roomGen(int r) {
    if(r == 0) {
        static int a[8] = {0b11100111, 0b11000011, 0b10000001, 0b00010100, 0b00000000, 0b10010101, 0b11000011, 0b11100111};
        return a;
    }          //winnable room
    else if(r == 1) {
        static int a[8] = {0b01001000, 0b01011010, 0b01001010, 0b01000010, 0b01011111, 0b01000000, 0b01111110, 0b00000000};
        return a;
    }
    else if(r == 2) {
        static int a[8] = {0b11000000, 0b11100000, 0b01110000, 0b00111000, 0b00011100, 0b00001110, 0b00000111, 0b00000011};
        return a;
    }
    else if(r == 3) {
        static int a[8] = {0b00000011, 0b00000111, 0b00001110, 0b00011100, 0b00111000, 0b01110000, 0b11100000, 0b11000000};
        return a;
    }
    else if(r == 4) {
        static int a[8] = {0b00001000, 0b01111000, 0b01000011, 0b01000010, 0b11000010, 0b00000010, 0b00111110, 0b00100000};
        return a;
    }
    else if(r == 5) {
        static int a[8] = {0b11100111, 0b11000011, 0b10011001, 0b00111100, 0b00111001, 0b10010011, 0b11000111, 0b11101111};
        return a;
    }
    else if(r == 6) {
        static int a[8] = {0b10000001, 0b00111101, 0b01100010, 0b00101110, 0b10100100, 0b01111110, 0b01010000, 0b00000101};
        return a;
    }
    else if(r == 7) {
        static int a[8] = {0b10000000, 0b10111110, 0b00100010, 0b01101010, 0b00101010, 0b01111010, 0b00100010, 0b10001000};
        return a;
    }
    else if(r == 8) {
        static int a[8] = {0b00000100, 0b10101010, 0b01000000, 0b01010100, 0b00001010, 0b10101010, 0b00010010, 0b01000100};
        return a;
    }
    else if(r == 9) {
        static int a[8] = {0b10000010, 0b01011000, 0b00100011, 0b01001010, 0b00001000, 0b00100100, 0b01010010, 0b10000001};
        return a;
    }
    else if(r == 10) {
        static int a[8] = {0b00100001, 0b00011100, 0b01000001, 0b01011010, 0b10011000, 0b10000011, 0b00101000, 0b00100101};
        return a;
    }
    else if(r == 11) {
        static int a[8] = {0b01000100, 0b00010000, 0b11111011, 0b00001010, 0b11101010, 0b00001010, 0b01111010, 0b00000000};
        return a;
    }
    else if(r == 12) {
        static int a[8] = {0b10101010, 0b00000000, 0b10101010, 0b00010100, 0b10101010, 0b00000001, 0b10111010, 0b00000100};
        return a;
    }
    else if(r == 13) {
        static int a[8] = {0b10101010, 0b00000001, 0b10101010, 0b00010000, 0b10101010, 0b00000001, 0b10101010, 0b01000101};
        return a;
    }
    else if(r == 14) {
        static int a[8] = {0b10101010, 0b00000000, 0b10111010, 0b00101000, 0b10111010, 0b00000000, 0b10101010, 0b00000000};
        return a;
    }
    else if(r == 15) {
        static int a[8] = {0b00000000, 0b11111110, 0b00000000, 0b01111110, 0b01111111, 0b00000000, 0b11111110, 0b00000000};
        return a;
    }
    else if(r == 16) {
        static int a[8] = {0b11101111, 0b10000001, 0b10101001, 0b00000000, 0b10101001, 0b10000001, 0b10000001, 0b11101111};
        return a;
    }
    else if(r == 17) {
        static int a[8] = {0b00000000, 0b01111110, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01111110, 0b00000000};
        return a;
    }
    else if(r == 18) {
        static int a[8] = {0b00101000, 0b00101000, 0b11101111, 0b00000000, 0b11101111, 0b00101000, 0b00101000, 0b00101000};
        return a;
    }
    else if(r == 19) {
        static int a[8] = {0b10000001, 0b01000010, 0b00100100, 0b00000000, 0b00001000, 0b00100100, 0b01000010, 0b10000001};
        return a;
    }
    else if(r == 20) {
        static int a[8] = {0b00000000, 0b00000000, 0b01001010, 0b01001000, 0b01111010, 0b01001010, 0b01001010, 0b00000000};
        return a;
    }
    else if(r == 21) {
        static int a[8] = {0b00000000, 0b00000000, 0b00011100, 0b00010000, 0b00011000, 0b00010000, 0b00011100, 0b00000000};
        return a;
    }
    else if(r == 22) {
        static int a[8] = {0b00100010, 0b00101010, 0b00101010, 0b00101010, 0b10101010, 0b10101010, 0b10101010, 0b10001000};
        return a;
    }
    else if(r == 23) {
        static int a[8] = {0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b00100100, 0b00100100, 0b00100100, 0b00100100};
        return a;
    }
    else if(r == 24) {
        static int a[8] = {0b00001000, 0b01000010, 0b00111100, 0b00111100, 0b10111101, 0b00111100, 0b01000010, 0b00001000};
        return a;
    }
    else if(r == 25) {
        static int a[8] = {0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00111100, 0b00000000, 0b00000000};
        return a;
    }
    else if(r == 26) {
        static int a[8] = {0b00000010, 0b00111010, 0b11000010, 0b00011000, 0b01011110, 0b01000010, 0b01101010, 0b00001000};
        return a;
    }
    else if(r == 27) {
        static int a[8] = {0b00000000, 0b00000000, 0b11111100, 0b00000100, 0b00000100, 0b11111100, 0b00000000, 0b00000000};
        return a;
    }
    else if(r == 28) {
        static int a[8] = {0b11100000, 0b11111110, 0b11100010, 0b01001000, 0b01011111, 0b01001111, 0b01101111, 0b00001111};
        return a;
    }
    else if(r == 29) {
        static int a[8] = {0b00000000, 0b00000000, 0b00011111, 0b00010000, 0b00010000, 0b00011111, 0b00000000, 0b00000000};
        return a;
    }
    else {
        static int a[8] = {0b11101111, 0b10000001, 0b10000001, 0b00000000, 0b10001001, 0b10000001, 0b10000001, 0b11101111};
        return a;
    }
}

void roomGenerator(){
    int r = rand() % diff;     //increase diff to increase difficulty
    while(r == lastRoom) {
        r = rand() % diff;
    }
    room = roomGen(r);
    lastRoom = r;
}

void printLvl() {
    char str[8];
    sprintf(str, "Lvl: %d", diff);
    lcd_setCursor(0, 0);
    lcd_printString(str,sizeof(str));
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
}

void initMazeGame(){
    room = start;
    x=0;
    y=0;
    flash=0;
    printLvl();
    LEDMatUpdateBuf(room);
    LEDMatWriteScreen();
}

void runMazeGame(){
        stickUpdate();
        
        LEDMatTurnOff(x, y);
    
        if(stickChanged()) {
            if(stickUp()) 
            {
                if(y != 0)
                {
                    if(!isItAWall(x, y-1))
                    {
                        y--;
                        flash = 0;
                    }
                }
                else if(x == 4 && lastDoor != 2)
                {
                    lastDoor = 1;
                    y = 7;
                    roomGenerator();
                    LEDMatUpdateBuf(room);
                    printLvl();
                    flash = 0;
                }
            }
            else if(stickDown()) 
            {
                if(y != 7)
                {    
                    if(!isItAWall(x, y+1))
                    {
                        y++;
                        flash = 0;
                    }
                }
                else if(x == 4 && lastDoor != 1)
                {
                    lastDoor = 2;
                    y = 0;
                    roomGenerator();
                    LEDMatUpdateBuf(room);
                    printLvl();
                    flash = 0;
                }
            }
            else if(stickLeft()) 
            {
                if(x != 7)
                {
                    if(!isItAWall(x+1, y))
                    {
                        x++;
                        flash = 0;
                    }
                }
                else if(y == 3 && lastDoor != 4)
                {
                    lastDoor = 3;
                    x = 0;
                    roomGenerator();
                    LEDMatUpdateBuf(room);
                    printLvl();
                    flash = 0;
                }
            }
            else if(stickRight()) 
            {
                if(x != 0)
                {
                    if(!isItAWall(x-1, y))
                    {
                        x--;
                        flash = 0;
                    }
                }
                else if(y == 3 && lastDoor != 3)
                {
                    lastDoor = 4;
                    x = 7;
                    roomGenerator();
                    LEDMatUpdateBuf(room);
                    printLvl();
                    flash = 0;
                }
            }
            else if(stickPressed()) {
                x = 0;
                y = 0;
                room = l;
                LEDMatUpdateBuf(room);
                printLvl();
                diff = 1;
                flash = 0;
            }
            if(x == 3 && y == 4)
            {
                diff++;           //scales infinitely
                x = 0;
                y = 0;
                lastDoor = 0;
                room = w;
                LEDMatUpdateBuf(room);
                printLvl();
                flash = 0;
            }
        }
        
        if(flash < 25) {
            LEDMatTurnOn(x, y);
            flash++;
        }
        else if(flash < 50) {
            flash++;
        }
        else {
            flash = 0;
        }
        
        LEDMatWriteScreen();
        while (!IFS1bits.T4IF);
        IFS1bits.T4IF = 0;
}