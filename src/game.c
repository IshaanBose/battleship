/*

File Name: game.c 
Author: Ishaan Bose
Date of Creation: 2021-10-12
____________________________________________________________________________________________________________________________________ 

    Contains functions for game operations.

    INCLUDES (user-defined)
    -----------------------
    1. easy_io.h
    2. player.h
    3. game.h

    GLOBAL VARIABLE
    ---------------
    1. char OPPPONENT_MOVE

    FUNCTIONS (Global)
    ------------------
    1. bool start(void)
    2. void convertToIndex(char* position, int* row, int* col)

    FUNCTIONS (Local)
    -----------------
    1. bool start()
    2. void takePosition(char* positionArr, char* positionPrompt)
    3. bool validatePosition(char* position)
    4. void placeShips(int playerNum)
    5. bool play(void)
    6. bool playTurn(int playerTurn)

*Compiled using C99 standards*

*/

#include <game.h>
#include <ctype.h>

char OPPONENT_MOVE = '\0'; // stores the status of the most recent move made by a player
bool CPU_PLAYER = false;
short PLAYER_TURN = 0;

void takePosition(char* positionArr, char* positionPrompt);
bool validatePosition(char* position);
void placeShips(void);
bool play(Difficulty difficulty);
bool playTurn(void);
void resetVariables(void);

void setCPUPlayer(bool isCPUPlayer)
{
    CPU_PLAYER = isCPUPlayer;
}

/*
    This functions starts the actual game of battleship. The function contains code that allows players to 
    place all their ships.

    Returns
    -------
    Returns `true` if after finishing the current game, player(s) want to keep playing, else it returns `false`.
*/
bool start(int playerTurn, Difficulty difficulty)
{
    if (CPU_PLAYER)
    {
        cpuPlaceShips(); // CPU places ships

        // player turn
        PLAYER_TURN = playerTurn;
        placeShips();
        displayBoard(players[PLAYER_TURN - 1].board);

        printf("All ships placed! Press any key to continue...");
        getchar();
        clearScreen();
    }
    else
    {
        for (int i = 1; i <= 2; i++)
        {
            PLAYER_TURN = i;
            placeShips();
            displayBoard(players[i - 1].board);

            if (i == 1)
                printf("\nAll ships placed! Give computer to player 2 so they can place their ships!\n");
            else
                printf("\nAll ships placed! Give computer back to player 1 so the game can begin!\n");

            printf("Press any key to continue...");
            getchar();
            clearScreen();
        }

        PLAYER_TURN = 1;
    }

    return play(difficulty);
}

/*
    This function contains code to actually play the game.

    Returns
    -------
    Returns `true` if player(s) want to keep playing after finishing the game, else it returns `false`.
*/
bool play(Difficulty difficulty)
{
    PLAYER_TURN = 1;
    bool showBoard = false, playUserTurn = false;

    while (1) // this loop will never be brokenn instead, control will be returned only after one of the players win.
    {
        if (CPU_PLAYER)
        {
            if ((getCPUTurn() + 1) == PLAYER_TURN) // if it is CPU's turn
            {
                if (playCPUTurn(&OPPONENT_MOVE, difficulty)) // if CPU wins
                {
                    printf("CPU Wins!\n");
                    resetVariables();

                    char playAgain;
                
                    while (1) // loop breaks after valid input ('Y' / 'N')
                    {
                        while (!checkInputValidity(
                            takeNInputWithPropmt("Play again? (Y/N): ", &playAgain, CHAR, 1),
                            NULL, "Please enter only 'Y' or 'N'.", NULL, NULL ));
                        
                        playAgain = tolower(playAgain);

                        if (playAgain != 'y' && playAgain != 'n')
                            printf("Please enter only 'Y' or 'N'.\n");
                        else
                            return playAgain == 'y';
                    }
                }
                else
                {
                    PLAYER_TURN = PLAYER_TURN == 1 ? PLAYER_TURN + 1 : PLAYER_TURN - 1; // next player's turn
                    continue;
                }
            }
        }

        printf("Player %d's turn (%s):\n", PLAYER_TURN, players[PLAYER_TURN - 1].name);

        if (showBoard)
        {
            char mergedBoard[BOARD_SIZE][BOARD_SIZE];
            mergeBoards(players[PLAYER_TURN - 1].board, players[PLAYER_TURN % 2].actionBoard, mergedBoard);
            displayBoard(mergedBoard);
            showBoard = false;
        }
        else if (playUserTurn)
        {
            if (playTurn()) // if one of the players win
            {
                printf("%s wins!\n", players[PLAYER_TURN - 1].name);
                resetVariables();
                
                char playAgain;
                
                while (1) // loop breaks after valid input ('Y' / 'N')
                {
                    while (!checkInputValidity(
                        takeNInputWithPropmt("Play again? (Y/N): ", &playAgain, CHAR, 1),
                        NULL, "Please enter only 'Y' or 'N'.", NULL, NULL ));
                    
                    playAgain = tolower(playAgain);

                    if (playAgain != 'y' && playAgain != 'n')
                        printf("Please enter only 'Y' or 'N'.\n");
                    else
                        return playAgain == 'y';
                }
            }

            playUserTurn = false;
            PLAYER_TURN = PLAYER_TURN == 1 ? PLAYER_TURN + 1 : PLAYER_TURN - 1; // next player's turn
            clearScreen();
            continue;
        }

        printf("\nOpponent's last move: ");
        printf(OPPONENT_MOVE != '\0' ? (OPPONENT_MOVE == 'H' ? "HIT!" : "MISS!") : "Not played");
        printf("\n1. Play Turn\n2. Show my board's status\n");

        while (1) // breaks after valid input (1 / 2)
        {
            short choice = 0;
            printf("Enter choice: ");

            checkInputValidity(
                takeNInput(&choice, SHORT, 1), 
                NULL, "Please enter only one digit.",
                "Please enter only a number.", NULL
            );

            switch (choice)
            {
                case 1:
                    playUserTurn = true;
                    break;
                
                case 2:
                    showBoard = true;
                    break;

                default:
                    printf("Enter only 1 or 2.\n\n");
                    break;
            }

            if (playUserTurn || showBoard) // breaking infinite loop after valid input
                break;
        }

        clearScreen();
    }
}

/*
    This function allows player to make a guess at where the opponent's ship is located.

    Parameter
    ---------
    `int playerTurn`:
        Contains player turn number (1 oe 2).
    
    Returns
    -------
    Returns `true` if the move makes player win, else returns `false`.
*/
bool playTurn()
{
    int playerTurn = PLAYER_TURN - 1; // converting to index friendly number
    char position[4];
    int row, col;

    displayBoard(players[playerTurn].actionBoard);
    
    while (1)
    {
        printf("\nEnter your guess: ");
        takePosition(position, "");
        convertToIndex(position, &row, &col);

        // checking if player already guessed at this location
        if (players[playerTurn].actionBoard[row][col] != ' ')
        {
            printf("Already guessed at this position.\n");
            continue;
        }

        break;
    }

    bool hit = false, shipSunk = false;
    char* sunkenShip;

    // checking if the location at which the guess was made contains a part of a ship
    if (players[(playerTurn + 1) % 2].board[row][col] != ' ')
    {
        players[playerTurn].actionBoard[row][col] = 'X';
        hit = true;

        // reducing ship hit points and checking if the respective ship has sank
        switch (players[(playerTurn + 1) % 2].board[row][col])
        {
            case 'C':
                if ((--players[(playerTurn + 1) % 2].shipsHP[CARRIER]) == 0)
                {
                    shipSunk = true;
                    sunkenShip = "Carrier";
                }
                break;
            case 'B':
                if ((--players[(playerTurn + 1) % 2].shipsHP[BATTLESHIP]) == 0)
                {
                    shipSunk = true;
                    sunkenShip = "Battleship";
                }
                break;
            case 'D':
                if ((--players[(playerTurn + 1) % 2].shipsHP[DESTROYER]) == 0)
                {
                    shipSunk = true;
                    sunkenShip = "Destroyer";
                }
                break;
            case 'S':
                if ((--players[(playerTurn + 1) % 2].shipsHP[SUBMARINE]) == 0)
                {
                    shipSunk = true;
                    sunkenShip = "Submarine";
                }
                break;
            case 'P':
                if ((--players[(playerTurn + 1) % 2].shipsHP[PATROL]) == 0)
                {
                    shipSunk = true;
                    sunkenShip = "Patrol";
                }
                break;
        }

        if (checkWin(playerTurn))
            return true;
    }
    else
    {
        players[playerTurn].actionBoard[row][col] = 'O';
    }

    clearScreen();

    printf("Player %d's turn (%s):\n", playerTurn + 1, players[playerTurn].name);
    displayBoard(players[playerTurn].actionBoard);
    printf(hit ? "\nHIT!\n" : "\nMISS!\n");
    OPPONENT_MOVE = hit ? 'H' : 'M';

    if (shipSunk)
    {
        printf("You sunk their %s!\n", sunkenShip);
    }

    printf("Press key to let player %d play...", ((playerTurn + 1) % 2) + 1);
    getchar();

    return false;
}

/*
    This functions prompts the user to place all the available ships.

    Parameter
    ---------
    `int playerNum`:
        Contains player number (1 oe 2).
*/
void placeShips()
{
    short shipsPlaced = 0;
    Ships shipsAvailable[] = { CARRIER, BATTLESHIP, DESTROYER, SUBMARINE, PATROL };

    while (shipsPlaced < 5)
    {
        printf("For %s:\n", players[PLAYER_TURN - 1].name);
        displayBoard(players[PLAYER_TURN - 1].board);

        printf("\nShips available:\n");

        for (int i = 0; i < SHIPS; i++)
        {
            switch (shipsAvailable[i]) // prints all available ships
            {
                case CARRIER:
                    printf("(C)arrier (5 spaces)\n");
                    break;
                case BATTLESHIP:
                    printf("(B)attleship (4 spaces)\n");
                    break;
                case DESTROYER:
                    printf("(D)estroyer (3 spaces)\n");
                    break;
                case SUBMARINE:
                    printf("(S)ubmarine (3 spaces)\n");
                    break;
                case PATROL:
                    printf("(P)atrol Boat (2 spaces)\n");
                    break;
            }
        }

        char choice;
        bool breakLoop = false;
        Ships ship;

        do
        {
            printf("Which piece do you want to place? (Type in the first letter of the piece): ");
            takeInput(&choice, CHAR);

            switch(tolower(choice))
            {
                case 'c':
                    if (shipsAvailable[CARRIER] == -1)
                    {
                        printf("Ship already placed!\n");
                        break;
                    }
                    shipsAvailable[CARRIER] = -1; // making selected ship unavailable
                    breakLoop = true;
                    ship = CARRIER;
                    break;
                case 'b':
                    if (shipsAvailable[BATTLESHIP] == -1)
                    {
                        printf("Ship already placed!\n");
                        break;
                    }
                    shipsAvailable[BATTLESHIP] = -1;
                    breakLoop = true;
                    ship = BATTLESHIP;
                    break;
                case 'd':
                    if (shipsAvailable[DESTROYER] == -1)
                    {
                        printf("Ship already placed!\n");
                        break;
                    }
                    shipsAvailable[DESTROYER] = -1;
                    breakLoop = true;
                    ship = DESTROYER;
                    break;
                case 's':
                    if (shipsAvailable[SUBMARINE] == -1)
                    {
                        printf("Ship already placed!\n");
                        break;
                    }
                    shipsAvailable[SUBMARINE] = -1;
                    breakLoop = true;
                    ship = SUBMARINE;
                    break;
                case 'p':
                    if (shipsAvailable[PATROL] == -1)
                    {
                        printf("Ship already placed!\n");
                        break;
                    }
                    shipsAvailable[PATROL] = -1;
                    breakLoop = true;
                    ship = PATROL;
                    break;
                default: printf("Please only enter a choice from the above given list.\n");
            }
        } while (!breakLoop);

        breakLoop = false;

        do
        {
            char startPosition[4], endPosition[4];
            takePosition(startPosition, "starting ");
            takePosition(endPosition, "ending ");

            if (canPlaceShipOnBoard(PLAYER_TURN, startPosition, endPosition, ship))
            {
                breakLoop = 1;
                shipsPlaced += 1;
            }
        } while (!breakLoop);

        clearScreen();
    } // end of while (shipsPlaced < 5), control comes out of this loop when all 5 ships have been placed
}

/*
    Allows user to enter a valid position on the game board.

    Parameters
    ----------
    `char* positionArr`:
        Array in which to store the valid position.
    
    `char* positionPrompt`:
        Function prompts user with string "Enter %sposition (ex: A5): ". positionArr replaces %s in this 
        string.
*/
void takePosition(char* positionArr, char* positionPrompt)
{
    short breakLoop = 0;

    do 
    {
        printf("Enter %sposition (ex: A5): ", positionPrompt);
    
        takeNInput(positionArr, STRING, 3);

        if (validatePosition(positionArr))
            breakLoop = 1;
        else
            printf("Incorrect input format. Please give input in the following range: [A-J][1-10].\n");

    } while (!breakLoop);
}

/*
    Checks if given position is valid with regards to the game board.

    Parameter
    ---------
    `char* position`:
        Position to validate.
    
    Returns
    -------
    Returns `true` if the position passed is valid, else returns `false`.
*/
bool validatePosition(char* position)
{
    bool flagCol = false, flagRow = false;

    if (isalpha(*position))
    {
        char col = toupper(*position);
        flagCol = (col >= 'A' && col <= 'J');
    }

    if (*(position + 1) != '\0')
    {
        if (isdigit(*(position + 1)))
        {
            flagRow = (*(position + 1) >= '1' && *(position + 1) <= '9');
        }
    }

    if (*(position + 2) != '\0' && flagRow)
    {
        flagRow = 0;

        if (isdigit(*(position + 1)))
        {
            flagRow = (*(position + 2) == '0');
        }
    }

    return flagRow && flagCol;
}

/*
    Converts the given position into its respective array index values.

    Parameters
    ----------
    `char* position`:
        The position to convert into index values.
    
    `int* row`:
        Variable in which to store the row value of the position.
    
    `int* col`:
        Variable in which to store the column value of the position.
*/
void convertToIndex(char* position, int* row, int* col)
{
    *col = ((int) toupper(*(position))) - 65;
    sscanf((position + 1), "%d", row); // converting number char into int
    *row -= 1;
}

void resetVariables()
{
    OPPONENT_MOVE = '\0';
    CPU_PLAYER = false;
    PLAYER_TURN = 0;

    for (int player = 0; player < 2; player++)
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                players[player].board[i][j] = ' ';
                players[player].actionBoard[i][j] = ' ';
            }
        }

        players[player].shipsHP[CARRIER] = 5; players[player].shipsHP[BATTLESHIP] = 4;
        players[player].shipsHP[DESTROYER] = 3; players[player].shipsHP[SUBMARINE] = 3;
        players[player].shipsHP[PATROL] = 2;
    }
}