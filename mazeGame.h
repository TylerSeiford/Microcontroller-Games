/* 
 * File:   mazegame.h
 * Author: Dylan
 *
 * Created on April 27, 2019, 7:34 PM
 */

#ifndef DILLO261_MAZEGAME_H
#define	DILLO261_MAZEGAME_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

    void wait();
    uint8_t isItAWall(uint8_t x, uint8_t y);
    int* roomGen(int r);
    void roomGenerator();
    void printLvl();
    void runMazeGame();
    void initMazeGame();

#ifdef	__cplusplus
}
#endif

#endif	/* DILLO261_MAZEGAME_H */

