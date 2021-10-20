/*

File Name: cpu.c 
Author: Ishaan Bose
Date of Creation: 2021-10-18
____________________________________________________________________________________________________________________________________ 



*Compiled using C99 standards*

*/

#include <cpu.h>
#include <stdio.h>

static int TURN = 0;

void setCPUTurn(int turn)
{
    TURN = turn;
}

int getCPUTurn()
{
    return TURN;
}

void cpuPlaceShips()
{
    Ships ships[] = { CARRIER, BATTLESHIP, DESTROYER, SUBMARINE, PATROL };
    int rowStart, colStart, rowEnd, colEnd, startArr[2], endArr[2];

    for (int i = 0; i < SHIPS; i++)
    {
        int shipHP = players[TURN].shipsHP[i];
        
        while (1)
        {
            bool validPositions = false;

            while (1) // for choosing starting position that has not been already occupied
            {
                rowStart = rand() % (BOARD_SIZE - shipHP); colStart = rand() % (BOARD_SIZE - shipHP);

                if (players[TURN].board[rowStart][colStart] == ' ')
                    break;
            }
            
            bool vertical = (rand() % 2) == 0 ? true : false;
            bool switched = false;

            while (1) // for choosing ending position that has not been already occupied
            { 
                if (vertical)
                {
                    rowEnd = rowStart + (shipHP - 1);
                    colEnd = colStart;
                }
                else
                {
                    rowEnd = rowStart;
                    colEnd = colStart + (shipHP - 1);
                }

                if (players[TURN].board[rowEnd][colEnd] == ' ')
                {
                    validPositions = true;
                    break;
                }
                else if (!switched) // if end position is not valid, then switch ship orientation and try again
                {
                    vertical = !vertical;
                    switched = true;
                }
                else // if we have already tried placing ship in both orientation, try again with another start position
                {
                    break;
                }
            }

            if (validPositions) // check if the positions are valid, if not, try again.
                break;
        }

        startArr[0] = rowStart; startArr[1] = colStart;
        endArr[0] = rowEnd; endArr[1] = colEnd;

        placeShipOnBoard(TURN + 1, startArr, endArr, ships[i]);
    }
}

bool playCPUTurnEasy(char* moveStatus)
{
    int row, col;

    while (1)
    {
        row = rand() % 10; col = rand() % 10; // generate a random position on the board

        if (players[TURN].actionBoard[row][col] == ' ') // if CPU hasn't guessed that position yet
            break;
    }

    if (players[(TURN + 1) % 2].board[row][col] != ' ') // if CPU hit a ship
    {
        players[TURN].actionBoard[row][col] = 'X';
        *moveStatus = 'H';

        switch (players[(TURN + 1) % 2].board[row][col])
        {
            case 'C':
                --players[(TURN + 1) % 2].shipsHP[CARRIER];
                break;
            case 'B':
                --players[(TURN + 1) % 2].shipsHP[BATTLESHIP];
                break;
            case 'D':
                --players[(TURN + 1) % 2].shipsHP[DESTROYER];
                break;
            case 'S':
                --players[(TURN + 1) % 2].shipsHP[SUBMARINE];
                break;
            case 'P':
                --players[(TURN + 1) % 2].shipsHP[PATROL];
                break;
        }
    }
    else
    {
        players[TURN].actionBoard[row][col] = 'O';
        *moveStatus = 'M';
    }

    return checkWin(TURN);
}