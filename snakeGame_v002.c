#include <xc.h>
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "drais007_LCD2_CLib_v001.h"
#include "mtdm_final_LEDMat_Clib.h"
#include "seifo002_stickLib.h"

#define slowness 20
#define X 0
#define Y 1
#define Dir 2

static uint8_t length;      //current length of snake
static bool lost;           //if they lost, true
static bool won;            //if they won, true
static uint8_t count;       //count to measure current amount of time
static int goalX;       //location to be eaten by snake
static int goalY;
static int tempX;
static int tempY;
static int tempDir;

static int snake[64][3]; //max size of snake, stores a x, y, and dir for each
static int head;
static int tail;

static int w[8] = {0b0,0b10000010,0b10010010,0b10101010,0b10101010,0b01000100,0b01000100,0b0};
static int l[8] = {0b0,0b00010000,0b00010000,0b00010000,0b00010000,0b00011100, 0b0, 0b0};

//------------------------conversions for testing equality----------------------
long convertLoc(int x, int y){//converts the location to a long
    return 0x0000000000000000 | (1<<x)<<(8*y);
}
long convertSnake(){//converts the entire snake to a long
    long snakeAsLong = 0x0000000000000000;
    int temp = tail;
    while(temp!=head){
        snakeAsLong = snakeAsLong | (1<<(snake[temp][X]))<<(8*(snake[temp][Y]));
        temp = (temp+1)%64;
    }
    return snakeAsLong;
}
bool collision(int x, int y){
    return (convertLoc(x, y) & convertSnake());
}

//-----------------------generates a new goal location--------------------------
void genGoal(){
    goalX= rand()%8;
    goalY = rand()%8;             //randomly decides on x and y
    long snake = convertSnake();
    while(convertLoc(goalX, goalY) & snake){  //ensures its not where the snake is--------test this!!!!!
        goalX=rand()%8;
        goalY=rand()%8;
    }
}

//-------------------------------LCD output and init----------------------------
/*void printLength() {
    char str[8];
    sprintf(str, "%d,%d    ", goalX, goalY);
    lcd_setCursor(0, 0);
    lcd_printStr(str);
}*/

void printLength() {
    char str[8];
    
    lcd_setCursor(0, 0);
    if(lost) {
        lcd_printChar('L');
    }
    else if(won) {
        lcd_printChar('W');
    }
    else {
        lcd_printChar(' ');
    }
    lcd_printChar(' ');
    sprintf(str, "%d,%d,%d", snake[head][X], snake[head][Y], snake[head][Dir]);

    lcd_printString(str,sizeof(str));
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
    
    
    lcd_setCursor(0, 1);
    sprintf(str, "%d,%d %d %x", goalX, goalY, length, count);
    lcd_printString(str,sizeof(str));
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
    lcd_printChar(' ');
}


void initSnakeGame(){
    lost = false;
    won = false;
    
    int i;
    for(i=0;i<64;i++){      //sets all of snake to 0;
        snake[i][X]=0;
        snake[i][Y]=0;
        snake[i][Dir]=0;
    }
    
    
    snake[0][X] = 4;
    snake[0][Y] = 4;
    snake[0][Dir] = 0;
    head = 0;
    tail = 0;
    
    tempX=0;
    tempY=0;
    tempDir=0;
    
    genGoal();                   //generates a new goal
    
    length = 1;                         //starting length is 1
    printLength();                      //prints length
    LEDMatClearScreen();
    LEDMatWriteScreen();                //clears the Matrix
}

void runSnakeGame(){
        stickUpdate();                  //checks for movement on joystick
        
        LEDMatTurnOff(goalX, goalY);    //toggles goal (for flashing)
        
        if(stickChanged()){             //sets direction if joystick moved
            if(stickDown()){
                if(snake[head][Dir]!=2)        //stops a direct reversal
                    snake[head][Dir] = 1;
            }
            else if(stickUp()) {
                if(snake[head][Dir]!=1)
                    snake[head][Dir] = 2;
            }
            else if(stickLeft()){
                if(snake[head][Dir]!=4)
                    snake[head][Dir] = 3;
            }
            else if(stickRight()){
                if(snake[head][Dir]!=3)
                    snake[head][Dir] = 4;
            }
            else if(stickPressed()){
                initSnakeGame();
            }
        }

         
        if(count > slowness) {
            if(!(lost||won)){
                if(snake[head][Dir] == 1){        //snake is moving up
                    if(snake[head][Y] <= 0){      //checks if moving out of bounds
                        lost = true;
                    }
                    else{
                        tempX = snake[head][X];
                        tempY = snake[head][Y]+1;
                        tempDir = snake[head][Dir];
                    }
                }
                else if(snake[head][Dir] == 2){   //moving down
                    if(snake[head][Y] >= 7){
                        lost = true;
                    }
                    else{
                        tempX = snake[head][X];
                        tempY = snake[head][Y]-1;
                        tempDir = snake[head][Dir];
                    }
                }
                else if(snake[head][Dir] == 3){   //moving left
                    if(snake[head][X] >= 7){
                        lost = true;
                    }
                    else{
                        tempX = snake[head][X]+1;
                        tempY = snake[head][Y];
                        tempDir = snake[head][Dir];
                    }
                }
                else if(snake[head][Dir] == 4){   //moving right
                    if(snake[head][X] <= 0){
                        lost = true;
                    }
                    else{
                        tempX = snake[head][X]-1;
                        tempY = snake[head][Y];
                        tempDir = snake[head][Dir];
                    }
                }
                //if the snake has moved from the start position, increment head
                if(snake[head][Dir]!=0){
                    if(collision(tempX, tempY))       //checks for a self collision
                        lost = true;
                    head = (head+1)%64;
                    snake[head][X] = tempX;
                    snake[head][Y] = tempY;
                    snake[head][Dir] = tempDir;
                }
                
                //updating the snakes path
                if(!((goalX == snake[head][X]) && (goalY == snake[head][Y]))){
                    LEDMatTurnOff(snake[tail][X], snake[tail][Y]);
                    tail = (tail+1)%64;
                }
                else{               //eats the goal and grows 1 length
                    genGoal();   //creates a new goal
                    length++;           //increases the length
                    printLength();      //prints new length
                    if(length == 64){     //win condition
                        won = true;
                    }
                }
                LEDMatTurnOn(snake[head][X], snake[head][Y]);
            }
            else if(won){          //tests for a win
                LEDMatUpdateBuf(w);
            }
            else if(lost){           //tests for a loss
                LEDMatUpdateBuf(l);
            }
            count = 0;              //resets count every "slowness" cycles
        }
        else if(count<(slowness/2)){    //toggles goal every "slowness"/2
            LEDMatTurnOn(goalX,goalY);
            count++;
        }
        else{
            count++;
        }
        
        
        LEDMatWriteScreen();
        printLength();
        while (!IFS1bits.T4IF);
        IFS1bits.T4IF = 0;
}

