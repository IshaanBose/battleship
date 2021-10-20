/*

File Name: cpu.h 
Author: Ishaan Bose
Date of Creation: 2021-10-18
____________________________________________________________________________________________________________________________________ 

Scenario:

    Header file for cpu.c.

*/

#include <stdlib.h>
#include <player.h>

void setCPUTurn(int turn);
int getCPUTurn(void);
void cpuPlaceShips(void);
bool playCPUTurnEasy(char* moveStatus);