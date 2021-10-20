/*

File Name: main.c 
Author: Ishaan Bose
Date of Creation: 2021-10-11
____________________________________________________________________________________________________________________________________ 

    This is main file for the battleship program. It only contains code for the startup menu. 

    INCLUDES (User-defined)
    -----------------------
    1. easy_io.h
    2. player.h
    3. game.h

    FUNCTIONS (Local)
    ----------------
    1. int main()
    2. int initMenu()

*Compiled using C99 standards*

*/

#include <game.h>
#include <ctype.h>
#include <time.h>

void initMenu();

int main()
{
    srand((unsigned int) time(NULL));
    initMenu();

    return 0;
}

void initMenu()
{
    while (1)
    {
        clearScreen();
        int choice = 0;
        bool keepPlaying = true;

        printf("==========================================================================\n");
        printf("|\t\t\t\t\t\t\t\t\t |\n");
        printf("|       \t\t\tBattleship\t\t\t         |\n");
        printf("|\t\t\t   Coded by: Ishaan Bose\t\t\t |\n");
        printf("|\t\t\t\t\t\t\t\t\t |\n");
        printf("==========================================================================\n");
        printf("\n1. Play PvP\n2. Play PvE\n3. Exit\n");

        while (1)
        {
            if (checkInputValidity(
                    takeNInputWithPropmt("Enter choice: ", &choice, INT, 1),
                    "Could not read input, try again.",
                    "Enter only a positive single digit number.",
                    "Enter only a number.", NULL
            ))
            {
                switch (choice)
                {
                    case 1:
                        createPlayer(1, false);
                        createPlayer(2, false);
                        clearScreen();

                        keepPlaying = start(0);

                        break;
                    
                    case 2:
                    {
                        char turnChoice;

                        while (1)
                        {
                            if (checkInputValidity(
                                takeNInputWithPropmt("Do you want to go first? (Y/N): ", &turnChoice, CHAR, 1),
                                NULL, "Enter only one character please.", NULL, NULL ));
                            turnChoice = tolower(turnChoice);
                            
                            if (turnChoice != 'y' && turnChoice != 'n')
                            {
                                printf("Please enter only 'Y' or 'N'.\n\n");
                                continue;
                            }

                            break;
                        }

                        int playerTurn = turnChoice == 'y' ? 1 : 2;
                        setCPUPlayer(true);
                        setCPUTurn(playerTurn % 2);

                        createPlayer(playerTurn == 1 ? 2 : 1, true); // creating CPU player
                        createPlayer(playerTurn, false);
                        clearScreen();
                        
                        keepPlaying = start(playerTurn);

                        break;
                    }

                    case 3: 
                        keepPlaying = false;
                        break;

                    default:
                        printf("Enter only numbers between 1 - 3\n");
                        continue;
                }

                if (keepPlaying)
                {
                    break; // breaks input loop
                }
                else
                {
                    printf("\n\n==========================================================================\n");
                    printf("|\t\t\t\t\t\t\t\t\t |\n");
                    printf("|\t\t\t   Thank you for playing!\t\t\t |\n");
                    printf("|\t\t\t\t\t\t\t\t\t |\n");
                    printf("==========================================================================\n\n");

                    exit(0);
                }
            }
        }
    }
}