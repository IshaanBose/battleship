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
void chooseDifficulty(Difficulty* difficulty);

int main()
{
    srand((unsigned int) time(NULL));
    initMenu();

    return 0;
}

void printBanner()
{
    clearScreen();
    printf("==========================================================================\n");
    printf("|\t\t\t\t\t\t\t\t\t |\n");
    printf("|       \t\t\tBattleship\t\t\t         |\n");
    printf("|\t\t\t   Coded by: Ishaan Bose\t\t\t |\n");
    printf("|\t\t\t\t\t\t\t\t\t |\n");
    printf("==========================================================================\n");
}

void mainMenu()
{
    printf("\n1. Play PvP\n2. Play PvE\n3. Exit\n");
}

void initMenu()
{
    while (1)
    {
        int choice = 0;
        bool keepPlaying = true;

        printBanner();
        mainMenu();

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

                        keepPlaying = start(0, 0);

                        break;
                    
                    case 2:
                    {
                        Difficulty difficulty;
                        chooseDifficulty(&difficulty);

                        if (difficulty == 3)
                        {
                            printBanner();
                            mainMenu();
                            continue;
                        }

                        printBanner();
                        char turnChoice;

                        while (1)
                        {
                            if (checkInputValidity(
                                takeNInputWithPropmt("\nDo you want to go first? (Y/N): ", &turnChoice, CHAR, 1),
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
                        
                        keepPlaying = start(playerTurn, difficulty);

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

void chooseDifficulty(Difficulty* difficulty)
{
    printBanner();
    printf("\nChoose difficulty:\n1. Easy\n2. Hard\n3. Go back\n");

    while (1)
    {
        while (!checkInputValidity(
            takeNInputWithPropmt("Enter difficulty: ", difficulty, SHORT, 1),
            NULL, "Enter only one number.", "Enter only a number.", NULL ));
        
        if (*difficulty < 1 || *difficulty > 3)
        {
            printf("Enter only numbers from 1 - 3.\n");
            continue;
        }

        break;
    }
}