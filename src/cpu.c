/*

File Name: cpu.c 
Author: Ishaan Bose
Date of Creation: 2021-10-18
____________________________________________________________________________________________________________________________________ 



*Compiled using C99 standards*

*/

#include <cpu.h>
#include <stack.h>
#include <stdio.h>
#include <string.h>

int TURN = 0;

// for hard difficulty

short lastMove = -1; // stores the position of the last successful hit
static struct Stack *checkout = NULL;
int hitCount = 0;
bool orientationFlipped = false;
/* 
    Direction in which to guess: 
    0 - no saved orientation, 1 - right, -1 - left, 10 - down, -10 - up
*/
short guessOrientation = 0; 

bool playCPUTurnEasy(char* moveStatus);
bool playCPUTurnHard(char* moveStatus);
void shuffleArray(int *array, int n);

void setCPUTurn(int turn)
{
    TURN = turn;
}

int getCPUTurn()
{
    return TURN;
}

void testCPU()
{
    setCPUTurn(1);
    strcpy(players[0].name, "user");
    cpuPlaceShips();
    char moveStatus;

    printf("Player board:\n");
    displayBoard(players[0].board);
    printf("CPU Board:\n");
    displayBoard(players[1].board);
    getchar();
    
    while (1)
    {
        if (playCPUTurnHard(&moveStatus))
        {
            printf("CPU WINS\n");
            exit(0);
        }
    }
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

bool playCPUTurn(char* moveStatus, Difficulty difficulty)
{
    switch (difficulty)
    {
        case EASY: return playCPUTurnEasy(moveStatus);
        case HARD: return playCPUTurnHard(moveStatus);
    }

    return true;
}

char playGuess(int row, int col, char *sunkShip)
{
    if (players[(TURN + 1) % 2].board[row][col] != ' ') // if CPU hit a ship
    {
        players[TURN].actionBoard[row][col] = 'X';

        switch (players[(TURN + 1) % 2].board[row][col])
        {
            case 'C':
                if (--players[(TURN + 1) % 2].shipsHP[CARRIER] == 0 && sunkShip != NULL)
                    *sunkShip = 'C';
                break;
            case 'B':
                if (--players[(TURN + 1) % 2].shipsHP[BATTLESHIP] == 0 && sunkShip != NULL)
                    *sunkShip = 'B';
                break;
            case 'D':
                if (--players[(TURN + 1) % 2].shipsHP[DESTROYER] == 0 && sunkShip != NULL)
                    *sunkShip = 'D';
                break;
            case 'S':
                if (--players[(TURN + 1) % 2].shipsHP[SUBMARINE] == 0 && sunkShip != NULL)
                    *sunkShip = 'S';
                break;
            case 'P':
                if (--players[(TURN + 1) % 2].shipsHP[PATROL] == 0 && sunkShip != NULL)
                    *sunkShip = 'P';
                break;
        }

        return 'H';
    }
    else
    {
        players[TURN].actionBoard[row][col] = 'O';
        return 'M';
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

    *moveStatus = playGuess(row, col, NULL);

    return checkWin(TURN);
}

/*
    Algorithm based on: https://www.datagenetics.com/blog/december32011/index.html (Parity)
*/
bool playCPUTurnHard(char* moveStatus)
{
    char sunk = '\0';
    
    if (checkout == NULL)
    {
        checkout = createStack(20);

        if (checkout == NULL)
        {
            printf("Could not create CPU stack.\n");
            exit(1);
        }
    }

    int row, col;

    // if there is no previously stored guessOrientation and stack is empty
    if (guessOrientation == 0 && isStackEmpty(checkout))
    {
        while (1)
        {
            row = rand() % 10; col = rand() % 10; // generate a random position on the board

            if ((row + col) % 2 == 0) // making sure to only hit odd cells
                continue;

            if (players[TURN].actionBoard[row][col] == ' ') // if CPU hasn't guessed that position yet
                break;
        }
    }
    else // if there has been a hit or there is a position to checkout
    {
        // if last guess was a hit and the checkout stack isn't empty and we have a guess orientation, no need to pop from stack
        if (lastMove != -1 && !isStackEmpty(checkout) && guessOrientation != 0)
        {
            row = (lastMove + guessOrientation) / 10;
            col = (lastMove + guessOrientation) % 10;

            if (row < 0 || row == BOARD_SIZE || col < 0 || col == BOARD_SIZE)
            {
                row = lastMove / 10; col = lastMove % 10;
                guessOrientation = (-guessOrientation);
                
                while (1)
                {
                    row = row + (guessOrientation / 10);
                    col = col + (guessOrientation % 10);

                    if (players[TURN].actionBoard[row][col] == 'O')
                    {
                        guessOrientation = 0;
                        return playCPUTurnHard(moveStatus);
                    }

                    if (players[TURN].actionBoard[row][col] == ' ')
                        break;
                }

                lastMove = ((row - (guessOrientation / 10)) * 10) + (col - (guessOrientation % 10));
            }
        }
        // if last move was a hit and stack isn't empty and we don't have a guess orientation
        // or if last move was a miss and stack isn't empty
        else
        {
            int guessCell = pop(checkout);
            row = guessCell / 10; col = guessCell % 10;
        }
    }

    *moveStatus = playGuess(row, col, &sunk);

    int orientations[4] = { 1, -1, 10, -10 };
    shuffleArray(orientations, 4);

    if (*moveStatus == 'H') // if guess resulted in a hit
    {
        short successfulMove = (row * 10) + col;
        hitCount++;

        if (sunk == '\0')
        {
            if (!isStackEmpty(checkout))
            {
                guessOrientation = successfulMove - lastMove;
            }

            // then we need to add adjacent cells to stack
            for (int i = 0; i < 4; i++)
            {
                if (orientations[i] == guessOrientation)
                    continue;
                
                int adjRow = (successfulMove / 10) + (orientations[i] / 10);
                int adjCol = (successfulMove % 10) + (orientations[i] % 10);

                if (adjRow < 0 || adjRow == BOARD_SIZE || adjCol < 0 || adjCol == BOARD_SIZE)
                    continue;
                
                if (players[TURN].actionBoard[adjRow][adjCol] != ' ')
                    continue;
                
                if (stackContains(checkout, (adjRow * 10) + adjCol))
                    continue;
                
                push(checkout, (adjRow * 10) + adjCol);
            }
        }
        else
        {
            switch (sunk)
            {
                case 'C':
                    hitCount -= 5;
                    break;

                case 'B':
                    hitCount -= 4;
                    break;
                
                case 'D':
                case 'S':
                    hitCount -= 3;
                    break;
                
                case 'P':
                    hitCount -= 2;
                    break;
            }

            sunk = '\0';

            if (hitCount == 0)
            {
                clearStack(checkout);
            }

            guessOrientation = 0;
        }

        lastMove = successfulMove;
    }
    else // if guess resulted in a miss
    {
        if (!orientationFlipped && guessOrientation != 0)
        {
            guessOrientation = (-guessOrientation);
            int tempRow = lastMove / 10, tempCol = lastMove % 10;

            while (1)
            {
                tempRow += (guessOrientation / 10);
                tempCol += (guessOrientation % 10);

                if (players[TURN].actionBoard[tempRow][tempCol] == ' ')
                {
                    tempRow -= (guessOrientation / 10);
                    tempCol -= (guessOrientation % 10);

                    lastMove = (tempRow * 10) + tempCol;

                    break;
                }
                else if (players[TURN].actionBoard[lastMove / 10][lastMove % 10] == 'O')
                {
                    guessOrientation = 0;
                    break;
                }
            }

            orientationFlipped = true;
        }
        else
        {
            guessOrientation = 0;
            orientationFlipped = false;
        }
    }

    return checkWin(TURN);
}

/*
    Function to shuffle an array of n size. Taken from: https://benpfaff.org/writings/clc/shuffle.html
*/
void shuffleArray(int *array, int n)
{
    if (n > 1)
    {
        for (int i = 0; i < n - 1; i++)
        {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void resetCPUVariables()
{
    destoryStack(checkout);
    
    TURN = 0;
    lastMove = -1;
    checkout = NULL;
    hitCount = 0;
}