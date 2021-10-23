/*

File Name: player.h
Author: Ishaan Bose
Date of Creation: 2021-10-12
____________________________________________________________________________________________________________________________________ 

Scenario:

    Header file for player.c.

    INCLUDES
    --------
    stdbool.h

    MACROS
    ------
    1. MAX_SIZE
    2. BOARD_SIZE
    3. SHIPS

    ENUM
    ----
    Ships

    STRUCT
    ------
    Player

    GLOBAL VARIABLE
    ---------------
    struct Player players[]

    FUNCTIONS
    ---------
    1. void createPlayer(int playerNum, bool createCPU)
    2. void displayBoard(char board[BOARD_SIZE][BOARD_SIZE])
    3. bool canPlaceShipOnBoard(int playerNum, char* startingPosition, char* endingPosition, Ships ship)
    4. bool placeShipOnBoard(int playerNum, int *startArr, int *endArr, Ships ship)
    5. bool checkWin(int playerTurn)
    6. void mergeBoards(
            char playerBoard[BOARD_SIZE][BOARD_SIZE], 
            char opponentActionBoard[BOARD_SIZE][BOARD_SIZE], 
            char merged[BOARD_SIZE][BOARD_SIZE]
        )

*/

#include <stdbool.h>

// macros
#define MAX_SIZE 50 // max size for name attribute in Player struct
#define BOARD_SIZE 10 // size of the game board
#define SHIPS 5 // no. of ships

// index values of ships
typedef enum {
    CARRIER,
    BATTLESHIP,
    DESTROYER,
    SUBMARINE,
    PATROL
} Ships;

/*
    Player struct, contains:
    - char name[MAX_SIZE]: contains the player's name.
    - char board[BOARD_SIZE][BOARD_SIZE]: array which stores the position of the player's ships.
    - char actionBoard[BOARD_SIZE][BOARD_SIZE]: array which stores player's guesses.
    - int shipsHP[SHIPS]: array which specifies the length of a ship.
*/
struct Player
{
    char name[MAX_SIZE];
    char board[BOARD_SIZE][BOARD_SIZE];
    char actionBoard[BOARD_SIZE][BOARD_SIZE];
    int shipsHP[SHIPS];
};

// array of Player structs, storing the two players playing the game.
extern struct Player players[2];

// functions

void createPlayer(int playerNum, bool createCPU);
void displayBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool canPlaceShipOnBoard(int playerNum, char* startingPosition, char* endingPosition, Ships ship);
bool placeShipOnBoard(int playerNum, int *startArr, int *endArr, Ships ship);
bool checkWin(int playerTurn);
void mergeBoards(
    char playerBoard[BOARD_SIZE][BOARD_SIZE], 
    char opponentActionBoard[BOARD_SIZE][BOARD_SIZE], 
    char merged[BOARD_SIZE][BOARD_SIZE]
);