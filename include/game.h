/*

File Name: game.h
Author: Ishaan Bose
Date of Creation: 2021-10-12
____________________________________________________________________________________________________________________________________ 

    Header file for game.c.

    INCLUDES
    --------
    1. easy_io.h
    2. cpu.h

    FUNCTIONS
    ---------
    1. void setCPUPlayer(void)
    2. bool start(int playerTurn, Difficulty difficulty)
    3. void convertToIndex(char* position, int* row, int* col)

*/

#include <easy_io.h>
#include <cpu.h>

// Functions

void setCPUPlayer(void);
bool start(int playerTurn, Difficulty difficulty);
void convertToIndex(char* position, int* row, int* col);