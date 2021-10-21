/*

File Name: game.h
Author: Ishaan Bose
Date of Creation: 2021-10-12
____________________________________________________________________________________________________________________________________ 

    Header file for game.c.

    FUNCTIONS
    ---------
    1. bool start(void)
    2. void convertToIndex(char* position, int* row, int* col)

*/

#include <easy_io.h>
#include <cpu.h>

// Functions

void setCPUPlayer(bool isCPUPlayer);
bool start(int playerTurn, Difficulty difficulty);
void convertToIndex(char* position, int* row, int* col);