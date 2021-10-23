/*

File Name: cpu.h 
Author: Ishaan Bose
Date of Creation: 2021-10-18
____________________________________________________________________________________________________________________________________ 

Scenario:

    Header file for cpu.c.

    INCLUDES
    --------
    1. stdlib.h
    2. player.h

    ENUM
    ----
    Difficulty

    FUNCTIONS
    ---------
    1. void setCPUTurn(int turn)
    2. int getCPUTurn(void)
    3. void cpuPlaceShips(void)
    4. bool playCPUTurn(char *moveStatus, Difficulty difficulty)
    5. void resetCPUVariables(void)

*/

#include <stdlib.h>
#include <player.h>

// CPU difficulties
typedef enum {
    EASY = 1,
    HARD
} Difficulty;

// functions

void setCPUTurn(int turn);
int getCPUTurn(void);
void cpuPlaceShips(void);
bool playCPUTurn(char *moveStatus, Difficulty difficulty);
void resetCPUVariables(void);