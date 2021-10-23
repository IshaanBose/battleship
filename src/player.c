/*

File Name: player.c 
Author: Ishaan Bose
Date of Creation: 2021-10-12
____________________________________________________________________________________________________________________________________ 

    Contains functions that allows for interaction with player properties.

    INCLUDES (user-defined)
    -----------------------
    game.h

    GLOBAL VARIABLE
    ---------------
    struct Player players[]

    FUNCTIONS (global)
    ------------------
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

*Compiled using C99 standards*

*/

#include <game.h>
#include <string.h>

// initiates default players values
struct Player players[] = {
    { 
        .name = "",
        .board = {
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
        },
        .actionBoard = {
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
        },
        .shipsHP = { 5, 4, 3, 3, 2 }
    },
    { 
        .name = "",
        .board = {
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
        },
        .actionBoard = {
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
            { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
        },
        .shipsHP = { 5, 4, 3, 3, 2 }
    }
};

/*
    This function creates a player, either a user-based player or a CPU-based player. It simply accepts the
    player name in case of it being a user-based player.

    Parameters
    ----------
    `int playerNum`:
        The player number to be created (1 or 2).
    
    `bool createCPU`:
        Specifies whether or not the player being created is CPU or not.
*/
void createPlayer(int playerNum, bool createCPU)
{
    char name[MAX_SIZE];

    if (!createCPU)
    {
        short validInp = 0;

        do
        {
            printf("Enter player %d's name: ", playerNum);

            validInp = checkInputValidity(
                takeNInput(name, STR_WHITESPC, MAX_SIZE),
                "Could not read input, try again.",
                "Name too long, please enter a name containing only 50 characters.",
                NULL, NULL);
        } while(!validInp);
    }

    strncpy(players[playerNum - 1].name, createCPU ? "CPU" : name, MAX_SIZE);
}

/*
    This function displays the specified game board.

    Parameter
    ---------
    `char board[BOARD_SIZE][BOARD_SIZE]`:
        The game board to be displayed.
*/
void displayBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    printf("\n     A    B    C    D    E    F    G    H    I    J  \n");
    printf("   --------------------------------------------------\n");

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (i != 9)
            printf(" %d ", i + 1);
        else
            printf("%d ", i + 1);

        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("| %c |", board[i][j]);
        }

        printf("\n");
    }

    printf("   --------------------------------------------------\n");
}

/*
    Merges the specified player's game board with the specified action board of the opponent player's actionn
    board, and stores it into the passed merged array. The merged board allows player to see which of their 
    ships have been hit and where.

    Parameters
    ----------
    `char playerBoard[BOARD_SIZE][BOARD_SIZE]`:
        The current player's game board, i.e., the board on which they placed their ships.
    
    `char opponentActionBoard[BOARD_SIZE][BOARD_SIZE]`:
        The opponent player's action board, i.e, the board on which the opponent player's guesses are stored.
    
    `char merged[BOARD_SIZE][BOARD_SIZE]`:
        The array in which to store the merged board.
*/
void mergeBoards(
    char playerBoard[BOARD_SIZE][BOARD_SIZE], 
    char opponentActionBoard[BOARD_SIZE][BOARD_SIZE], 
    char merged[BOARD_SIZE][BOARD_SIZE]
)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (playerBoard[i][j] == 'C' || playerBoard[i][j] == 'B' || playerBoard[i][j] == 'D' 
                || playerBoard[i][j] == 'S' || playerBoard[i][j] == 'P')
            {
                if (opponentActionBoard[i][j] != ' ')
                {
                    merged[i][j] = opponentActionBoard[i][j];
                    continue;
                }
            }
            
            merged[i][j] = playerBoard[i][j];
        }
    }
}


/*
    This function checks whether the given positions can fit the given ship and place it if does.

    Parameters
    ----------
    `int playerNum`:
        Contains player number (1 or 2).
    
    `char *startingPosition`:
        Starting coordinates from where to start placing the ship.

    `char *endingPosition`:
        Ending coordinates from where to stop placing the ship.

    `Ships ships`:
        Ship to place.
    
    Returns
    -------
    Returns `true` if the ship was successfully placed on the player's board, else it returns `false`.
*/
bool canPlaceShipOnBoard(int playerNum, char *startingPosition, char *endingPosition, Ships ship)
{
    int startRow, startCol, endRow, endCol, temp;
    int startArr[2], endArr[2];

    convertToIndex(startingPosition, &startRow, &startCol);
    convertToIndex(endingPosition, &endRow, &endCol);

    temp = startRow;
    startRow = startRow < endRow ? startRow : endRow;
    endRow = temp >= endRow ? temp : endRow;

    temp = startCol;
    startCol = startCol < endCol ? startCol : endCol;
    endCol = temp >= endCol ? temp : endCol;

    if (players[playerNum - 1].board[startRow][startCol] != ' ')
    {
        printf("%s is already occupied, please enter another.\n\n", startingPosition);
        return false;
    }
    if (players[playerNum - 1].board[endRow][endCol] != ' ')
    {
        printf("%s is already occupied, please enter another.\n\n", endingPosition);
        return false;
    }
    if ((startRow != endRow) && (startCol != endCol))
    {
        printf("Cannot place ships diagonally\n\n");
        return false;
    }
    if ((endCol - startCol) + 1 != players[playerNum - 1].shipsHP[ship] 
            && (endRow - startRow) + 1 != players[playerNum - 1].shipsHP[ship])
    {
        printf("Cannot fit ship in given positions.\n\n");
        return false;
    }

    if (startRow == endRow)
    {
        for (int i = startCol; i <= endCol; i++)
        {
            if (players[playerNum - 1].board[startRow][i] != ' ')
            {
                printf("Ships cannot overlap.\n\n");
                return false;
            }
        }
    }
    else
    {
        for (int i = startRow; i <= endRow; i++)
        {
            if (players[playerNum - 1].board[i][startCol] != ' ')
            {
                printf("Ships cannot overlap.\n\n");
                return false;
            }
        }
    }

    startArr[0] = startRow; startArr[1] = startCol;
    endArr[0] = endRow; endArr[1] = endCol;

    return placeShipOnBoard(playerNum, startArr, endArr, ship);
}

/*
    Places the specified ship on the board between the specified positions.

    Parameters
    ----------
    `int playerNum`:
        Contains player number (1 or 2).
    
    `char *startingPosition`:
        Starting coordinates from where to start placing the ship.

    `char *endingPosition`:
        Ending coordinates from where to stop placing the ship.

    `Ships ships`:
        Ship to place.
*/
bool placeShipOnBoard(int playerNum, int *startArr, int *endArr, Ships ship)
{
    char shipIcon = '\0';

    switch (ship)
    {
        case CARRIER:
            shipIcon = 'C';
            break;
        
        case BATTLESHIP:
            shipIcon = 'B';
            break;
        
        case DESTROYER:
            shipIcon = 'D';
            break;

        case SUBMARINE:
            shipIcon = 'S';
            break;
        
        case PATROL:
            shipIcon = 'P';
            break;

        default: printf("Invalid ship!\n");
            break;
    }

    for (int i = startArr[0], j = startArr[1]; !(i == endArr[0] && j == endArr[1]);)
    {
        players[playerNum - 1].board[i][j] = shipIcon;

        if (i != endArr[0])
            i++;
        if (j != endArr[1])
            j++;
    }

    players[playerNum - 1].board[endArr[0]][endArr[1]] = shipIcon;

    return true;
}

/*
    Checks if the specified player has won the game or not.

    Parameter
    ---------
    `int playerTurn`:
        Contains player turn number (0 or 1).
    
    Returns
    -------
    Returns `true` if player has won, else it returns `false`.
*/
bool checkWin(int playerTurn)
{
    for (int i = 0; i < SHIPS; i++)
    {
        if (players[(playerTurn + 1) % 2].shipsHP[i] != 0)
            return false;
    }

    return true;
}