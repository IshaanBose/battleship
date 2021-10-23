/*

File Name: cpu.c 
Author: Ishaan Bose
Date of Creation: 2021-10-18
____________________________________________________________________________________________________________________________________ 

    Contains functions for CPU player operations.

    INCLUDES (user-defined)
    1. cpu.h
    2. stack.h

    GLOBAL VARIABLES
    ----------------
    1. int  _CPUTurn
    2. short _lastMove
    3. struct Stack *_checkout
    4. int _hitCount
    5. bool _orientationFlipped
    6. short _guessOrientation

    FUNCTIONS (global)
    ------------------
    1. void setCPUTurn(int turn)
    2. int getCPUTurn(void)
    3. void cpuPlaceShips(void)
    4. bool playCPUTurn(char *moveStatus, Difficulty difficulty)
    5. void resetCPUVariables()
    6. void testCPU()

    FUNCTIONS (local)
    -----------------
    1. bool playCPUTurnEasy(char *moveStatus)
    2. bool playCPUTurnHard(char *moveStatus)
    3. void shuffleArray(int *array, int n)
    4. char playGuess(int row, int col, char *sunkShip)

*Compiled using C99 standards*

*/

#include <cpu.h>
#include <stack.h>
#include <stdio.h>
#include <string.h>

// stores the CPU's turn
int _CPUTurn = 0;

/* For hard difficulty */

// stores the position of the last successful hit
short _lastMove = -1;
// stack that stores cells for the CPU to explore
struct Stack *_checkout = NULL;
// counts how many times the CPU has guessed successfully
int _hitCount = 0;
// checks whether _guessOrientation has flipped already or not
bool _orientationFlipped = false;
/* 
    Direction in which to guess: 
    0 - no saved orientation, 1 - right, -1 - left, 10 - down, -10 - up
*/
short _guessOrientation = 0; 

bool playCPUTurnEasy(char *moveStatus);
bool playCPUTurnHard(char *moveStatus);
void shuffleArray(int *array, int n);

/*
    Used to set CPU's turn.

    Parameter
    ---------
    `int turn`:
        CPU's turn, either 0 or 1.
*/
void setCPUTurn(int turn)
{
    _CPUTurn = turn;
}

/*
    Returns the CPU turn.
*/
int getCPUTurn()
{
    return _CPUTurn;
}

/*
    Randomly places ships on the CPU's board.
*/
void cpuPlaceShips()
{
    Ships ships[] = { CARRIER, BATTLESHIP, DESTROYER, SUBMARINE, PATROL };
    int rowStart, colStart, rowEnd, colEnd, startArr[2], endArr[2];

    for (int i = 0; i < SHIPS; i++)
    {
        int shipHP = players[_CPUTurn].shipsHP[i];
        
        while (1)
        {
            bool validPositions = false;

            while (1) // for choosing starting position that has not been already occupied
            {
                rowStart = rand() % (BOARD_SIZE - shipHP); colStart = rand() % (BOARD_SIZE - shipHP);

                if (players[_CPUTurn].board[rowStart][colStart] == ' ')
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

                if (players[_CPUTurn].board[rowEnd][colEnd] == ' ')
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

        placeShipOnBoard(_CPUTurn + 1, startArr, endArr, ships[i]);
    }
}

/*
    Plays the CPU's turn.

    Parameters
    ----------
    `char *moveStatus`:
        Used to tell user the status of the CPU's move.
    
    `Difficulty difficulty`:
        Selects the difficulty of the CPU.
    
    Returns
    -------
    Returns `true` if CPU won, else it returns `false`.
*/
bool playCPUTurn(char *moveStatus, Difficulty difficulty)
{
    switch (difficulty)
    {
        case EASY: return playCPUTurnEasy(moveStatus);
        case HARD: return playCPUTurnHard(moveStatus);
    }

    return true;
}

/*
    Plays the CPU's guess.

    Parameters
    ----------
    `int row`:
        Row of the guessed cell.
    
    `int col`:
        Column of the guessed cell.
    
    `char *sunkShip`:
        Stores the value of the ship which has sunk, if any.
    
    Returns
    -------
    Returns 'H' if the guess results in a hit, else it returns 'M'.
*/
char playGuess(int row, int col, char *sunkShip)
{
    if (players[(_CPUTurn + 1) % 2].board[row][col] != ' ') // if CPU hit a ship
    {
        players[_CPUTurn].actionBoard[row][col] = 'X';

        switch (players[(_CPUTurn + 1) % 2].board[row][col])
        {
            case 'C':
                if (--players[(_CPUTurn + 1) % 2].shipsHP[CARRIER] == 0 && sunkShip != NULL)
                    *sunkShip = 'C';
                break;
            case 'B':
                if (--players[(_CPUTurn + 1) % 2].shipsHP[BATTLESHIP] == 0 && sunkShip != NULL)
                    *sunkShip = 'B';
                break;
            case 'D':
                if (--players[(_CPUTurn + 1) % 2].shipsHP[DESTROYER] == 0 && sunkShip != NULL)
                    *sunkShip = 'D';
                break;
            case 'S':
                if (--players[(_CPUTurn + 1) % 2].shipsHP[SUBMARINE] == 0 && sunkShip != NULL)
                    *sunkShip = 'S';
                break;
            case 'P':
                if (--players[(_CPUTurn + 1) % 2].shipsHP[PATROL] == 0 && sunkShip != NULL)
                    *sunkShip = 'P';
                break;
        }

        return 'H';
    }
    else
    {
        players[_CPUTurn].actionBoard[row][col] = 'O';
        return 'M';
    }
}

/*
    Plays the CPU turn on easy difficulty. On easy difficulty, the CPU simply randomly guesses.

    Parameter
    ---------
    `char *moveStatus`:
        Stores the status of the guess, either 'H' or 'M'.
    
    Returns
    -------
    Returns `true` if the CPU wins, else it returns `false`.
*/
bool playCPUTurnEasy(char *moveStatus)
{
    int row, col;

    while (1)
    {
        row = rand() % 10; col = rand() % 10; // generate a random position on the board

        if (players[_CPUTurn].actionBoard[row][col] == ' ') // if CPU hasn't guessed that position yet
            break;
    }

    *moveStatus = playGuess(row, col, NULL);

    return checkWin(_CPUTurn);
}

/*
    Plays the CPU turn on hard difficulty. Uses a modified version of the algorithm described here: 
    https://www.datagenetics.com/blog/december32011/index.html (Hunt (with parity)/Target).
    
    Initially, the CPU randomly guesses odd cells.
    The modified algorithm uses variable _guessOrientation to track successive hits in order to keep guessing the same direction.
    If the current orientation results in going out of bounds of the board or if it results in a miss, then the orientation is flipped.
    _lastMove keeps track of the last successful move. _checkout is a stack that stores the cells adjacent to a successful hit, it is used
    to take better informed guesses. _hitCount keeps track of how many successful hits the CPU has had, if the CPU sinks a ships, the length
    of the ship is subtracted from _hitCount, if _hitCount reaches 0, the stack is emptied.

    Parameter
    ---------
    `char *moveStatus`:
        Stores the status of the guess, either 'H' or 'M'.
    
    Returns
    -------
    Returns `true` if the CPU wins, else it returns `false`.
*/
bool playCPUTurnHard(char *moveStatus)
{
    char sunk = '\0'; // variable to check whether the CPU sank a ship
    
    if (_checkout == NULL) // create a checkout stack if it doesn't already exist
    {
        _checkout = createStack(20);

        if (_checkout == NULL)
        {
            printf("Could not create CPU stack.\n");
            exit(1);
        }
    }

    int row, col; // row and column of the current guess

    // if there is no previously stored _guessOrientation and stack is empty then the CPU will randomly select an odd numbered cell
    if (_guessOrientation == 0 && isStackEmpty(_checkout))
    {
        while (1)
        {
            row = rand() % 10; col = rand() % 10; // generate a random position on the board

            if ((row + col) % 2 == 0) // making sure to only hit odd cells
                continue;

            if (players[_CPUTurn].actionBoard[row][col] == ' ') // if CPU hasn't guessed that position yet, then stop randomly choosing
                break;
        }
    }
    else // if a _guessOrientation had been stored previously
    {
        // if last guess was a hit and the _checkout stack isn't empty and we have a guess orientation, no need to pop from stack
        if (_lastMove != -1 && !isStackEmpty(_checkout) && _guessOrientation != 0)
        {
            row = (_lastMove + _guessOrientation) / 10;
            col = (_lastMove + _guessOrientation) % 10;

            // if new cell goes beyond the boundaries of the game board, we try and flip the _guessOrientation
            if (row < 0 || row == BOARD_SIZE || col < 0 || col == BOARD_SIZE)
            {
                row = _lastMove / 10; col = _lastMove % 10;
                _guessOrientation = (-_guessOrientation);
                
                // keep moving in the flipped orientation until we find an empty cell or a missed cell
                while (1)
                {
                    row = row + (_guessOrientation / 10);
                    col = col + (_guessOrientation % 10);

                    // if the flip results in the finding of a missed cell, then we reset _guessOrientation and play the turn again
                    if (players[_CPUTurn].actionBoard[row][col] == 'O')
                    {
                        _guessOrientation = 0;
                        return playCPUTurnHard(moveStatus);
                    }

                    if (players[_CPUTurn].actionBoard[row][col] == ' ')
                        break;
                }

                // changes _lastMove so that it becomes the position before the missed cell
                _lastMove = ((row - (_guessOrientation / 10)) * 10) + (col - (_guessOrientation % 10));
            }
        }
        // if last move was a hit and stack isn't empty and we don't have a guess orientation
        // or if last move was a miss and stack isn't empty and _guessOrientation = 0
        else
        {
            int guessCell = pop(_checkout);
            row = guessCell / 10; col = guessCell % 10;
        }
    }

    *moveStatus = playGuess(row, col, &sunk);

    int orientations[4] = { 1, -1, 10, -10 };
    shuffleArray(orientations, 4); // shuffles the orientations array

    if (*moveStatus == 'H') // if guess resulted in a hit
    {
        short successfulMove = (row * 10) + col;
        _hitCount++;

        if (sunk == '\0')
        {
            if (!isStackEmpty(_checkout))
            {
                _guessOrientation = successfulMove - _lastMove;
            }

            // then we need to add adjacent cells to stack
            for (int i = 0; i < 4; i++)
            {
                if (orientations[i] == _guessOrientation)
                    continue;
                
                int adjRow = (successfulMove / 10) + (orientations[i] / 10);
                int adjCol = (successfulMove % 10) + (orientations[i] % 10);

                if (adjRow < 0 || adjRow == BOARD_SIZE || adjCol < 0 || adjCol == BOARD_SIZE)
                    continue;
                
                if (players[_CPUTurn].actionBoard[adjRow][adjCol] != ' ')
                    continue;
                
                if (stackContains(_checkout, (adjRow * 10) + adjCol))
                    continue;
                
                push(_checkout, (adjRow * 10) + adjCol);
            }
        }
        else
        {
            switch (sunk)
            {
                case 'C':
                    _hitCount -= 5;
                    break;

                case 'B':
                    _hitCount -= 4;
                    break;
                
                case 'D':
                case 'S':
                    _hitCount -= 3;
                    break;
                
                case 'P':
                    _hitCount -= 2;
                    break;
            }

            sunk = '\0';

            if (_hitCount == 0)
            {
                clearStack(_checkout);
            }

            _guessOrientation = 0;
        }

        _lastMove = successfulMove;
    }
    else // if guess resulted in a miss
    {
        if (!_orientationFlipped && _guessOrientation != 0)
        {
            _guessOrientation = (-_guessOrientation);
            int tempRow = _lastMove / 10, tempCol = _lastMove % 10;

            while (1)
            {
                tempRow += (_guessOrientation / 10);
                tempCol += (_guessOrientation % 10);

                if (players[_CPUTurn].actionBoard[tempRow][tempCol] == ' ')
                {
                    tempRow -= (_guessOrientation / 10);
                    tempCol -= (_guessOrientation % 10);

                    _lastMove = (tempRow * 10) + tempCol;

                    break;
                }
                else if (players[_CPUTurn].actionBoard[_lastMove / 10][_lastMove % 10] == 'O')
                {
                    _guessOrientation = 0;
                    break;
                }
            }

            _orientationFlipped = true;
        }
        else
        {
            _guessOrientation = 0;
            _orientationFlipped = false;
        }
    }

    return checkWin(_CPUTurn);
}

/*
    Function to shuffle an array of n size. Taken from: https://benpfaff.org/writings/clc/shuffle.html

    Parameters
    ----------
    `int *array`:
        Array to shuffle.
    
    `int n`:
        Size of array
*/
void shuffleArray(int *array, int size)
{
    if (size > 1)
    {
        for (int i = 0; i < size - 1; i++)
        {
            int j = i + rand() / (RAND_MAX / (size - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

/*
    Resets all the CPU variables.
*/
void resetCPUVariables()
{
    destoryStack(_checkout);
    
    _CPUTurn = 0;
    _lastMove = -1;
    _checkout = NULL;
    _hitCount = 0;
}