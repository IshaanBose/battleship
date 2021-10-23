/*

File Name: easy_io.c
Author: Ishaan Bose
Date of Creation: 2021-09-24
____________________________________________________________________________________________________________________________________ 

    Contains functions to make I/O operations easier to perform as well as easier to validate.

    INCLUDES (User-defined)
    ----------------------
    1. easy_io.h

    FUNCTIONS (Global)
    ------------------
    1. ReturnCode takeInput(void *inputVar, DataType type)
    2. ReturnCode takeNInput(void *inputVar, DataType type, int inputSize)
    3. ReturnCode takeInputWithPropmt(char *propmt, void *inputVar, DataType type)
    4. ReturnCode takeNInputWithPropmt(char *propmt, void *inputVar, DataType type, int inputSize)
    5. void clearScreen(void)
    6. short checkInputValidity(
            ReturnCode returnCode, 
            char *cannotReadMessage, 
            char *tooLongMessage, 
            char *invalidTypeMessage, 
            char *notSupportedMessage
        )

*Compiled using C99 standards*

*/

#include <easy_io.h>
#include <stdlib.h>

/*
    This macro defines a function CLSCR() which contains system specific code to clear screen.
    Works for Windows and Linux OSs.
*/
#ifdef _WIN32
    #define CLSCR() system("cls")
#else
    #define CLSCR() system("clear")
#endif

/*
    This function allows user to take an input of size 100.
    NOTE: All error codes are negative integers, so if you want to check for errors, check if the return value 
    is < 0. Alternatively, use the `checkInputValidity` function.

    Parameters
    ----------
    `void *inputVar`:
        A void pointer which contains memory address of variable in which to store user input.
    
   `DataType type`:
        Defines the type of the input. Supports the following types:
            1. char -> CHAR
            2. String w/ whitespaces -> STR_WHITESPC
            3. String -> STRING
            4. int -> INT
            5. short -> SHORT
            6. float -> FLOAT
    
    Returns
    -------
    Returns `OK` if valid input given, 
    else returns a value of datatype ReturnCode with one of the following values:
        `ERR_TOO_LONG` - if given input exceeds given input size.
        `ERR_INVALID_TYPE` - if given input if of the incorrect data type.
        `ERR_CANNOT_READ` - if for whatever reason, fgets was not able to reed from stdin.
        `ERR_DATATYPE_NOT_SUPPORTED` - if given value for type is not supported.
*/
ReturnCode takeInput(void *inputVar, DataType type)
{
    return takeNInput(inputVar, type, 100);
}


/*
    This function allows user to print a prompt message before taking an input of size 100.
    NOTE: All error codes are negative integers, so if you want to check for errors, check if the return value 
    is < 0. Alternatively, use the `checkInputValidity` function.

    Parameters
    ----------
    `char *prompt`:
        Contains the message to be printed before taking input.

    `void *inputVar`:
        A void pointer which contains memory address of variable in which to store user input.
    
   `DataType type`:
        Defines the type of the input. Supports the following types:
            1. char -> CHAR
            2. String w/ whitespaces -> STR_WHITESPC
            3. String -> STRING
            4. int -> INT
            5. short -> SHORT
            6. float -> FLOAT
    
    Returns
    -------
    Returns `OK` if valid input given, 
    else returns a value of datatype ReturnCode with one of the following values:
        `ERR_TOO_LONG` - if given input exceeds given input size.
        `ERR_INVALID_TYPE` - if given input if of the incorrect data type.
        `ERR_CANNOT_READ` - if for whatever reason, fgets was not able to reed from stdin.
        `ERR_DATATYPE_NOT_SUPPORTED` - if given value for type is not supported.
*/
ReturnCode takeInputWithPropmt(char *propmt, void *inputVar, DataType type)
{
    printf("%s", propmt);
    return takeInput(inputVar, type);
}

/*
    This function allows user to take an input of given size.
    NOTE: All error codes are negative integers, so if you want to check for errors, check if the return value 
    is < 0. Alternatively, use the checkInputValidity function.

    Parameters
    ----------
    `void *inputVar`:
        A void pointer which contains memory address of variable in which to store user input.
    
    `DataType type`:
        Defines the type of the input. Supports the following types:
            1. char -> CHAR
            2. String w/ whitespaces -> STR_WHITESPC
            3. String -> STRING
            4. int -> INT
            5. short -> SHORT
            6. float -> FLOAT
    
    `int inputSize`:
        Defines the size of the input.
    
    Returns
    -------
    Returns `OK` if valid input given, 
    else returns a value of datatype ReturnCode with one of the following values:
        `ERR_TOO_LONG` - if given input exceeds given input size.
        `ERR_INVALID_TYPE` - if given input if of the incorrect data type.
        `ERR_CANNOT_READ` - if for whatever reason, fgets was not able to reed from stdin.
        `ERR_DATATYPE_NOT_SUPPORTED` - if given value for type is not supported.

*/
ReturnCode takeNInput(void *inputVar, DataType type, int inputSize)
{
    char buffer[inputSize + 2];
    buffer[sizeof buffer - 1] = 'x';

    if (fgets(buffer, sizeof buffer, stdin))
    {
        if (buffer[sizeof buffer - 1] == '\0' && buffer[sizeof buffer - 2] != '\n')
        {
            // clearing stdin buffer
            int dump;
            while ((dump = fgetc(stdin)) != '\n' && dump != EOF);

            return ERR_TOO_LONG;
        }

        switch (type)
        {
            case INT:
                if (1 == sscanf(buffer, "%d", ((int*) inputVar)))
                    return OK;
                else
                    return ERR_INVALID_TYPE;
            case FLOAT:
                if (1 == sscanf(buffer, "%f", ((float*) inputVar)))
                    return OK;
                else
                    return ERR_INVALID_TYPE;
            case SHORT:
                if (1 == sscanf(buffer, "%hi", ((short*) inputVar)))
                    return OK;
                else
                    return ERR_INVALID_TYPE;
            case CHAR: return sscanf(buffer, "%c", ((char*) inputVar));
            case STR_WHITESPC: return sscanf(buffer, "%[^\n]s", ((char*) inputVar));
            case STRING: return sscanf(buffer, "%s", ((char*) inputVar));

            default: return ERR_DATATYPE_NOT_SUPPORTED;
        }
    }

    return ERR_CANNOT_READ;
}

/*
    This function allows user to print a prompt message before taking an input of given size.
    NOTE: All error codes are negative integers, so if you want to check for errors, check if the return value 
    is < 0. Alternatively, use the `checkInputValidity` function.

    Parameters
    ----------
    `char *prompt`:
        Contains the message to be printed before taking input.

    `void *inputVar`:
        A void pointer which contains memory address of variable in which to store user input.
    
   `DataType type`:
        Defines the type of the input. Supports the following types:
            1. char -> CHAR
            2. String w/ whitespaces -> STR_WHITESPC
            3. String -> STRING
            4. int -> INT
            5. short -> SHORT
            6. float -> FLOAT
    
    `int inputSize`:
        Defines the size of the input.
    
    Returns
    -------
    Returns `OK` if valid input given, 
    else returns a value of datatype ReturnCode with one of the following values:
        `ERR_TOO_LONG` - if given input exceeds given input size.
        `ERR_INVALID_TYPE` - if given input if of the incorrect data type.
        `ERR_CANNOT_READ` - if for whatever reason, fgets was not able to reed from stdin.
        `ERR_DATATYPE_NOT_SUPPORTED` - if given value for type is not supported.
*/
ReturnCode takeNInputWithPropmt(char *propmt, void *inputVar, DataType type, int inputSize)
{
    printf("%s", propmt);
    return takeNInput(inputVar, type, inputSize);
}

/*
    This function allows for automatic inference of the value returned by takeInput or takeNInput functions.

    Parameters
    ----------
    `int returnCode`:
        Value returned by takeInput or takeNInput functions.
    
    `char *cannotReadMessage`:
        Message to print in case return value is ERR_CANNOT_READ.
    
    `char *tooLongMessage`:
        Message to print in case return value is ERR_TOO_LONG.
    
    `char *invalidTypeMessage`:
        Message to print in case return value is ERR_INVALID_TYPE.
    
    `char *notSupportedMessage`:
        Message to print in case return value is ERR_DATATYPE_NOT_SUPPORTED.
    
    Returns
    -------
    Returns 1 if no errors, else returns 0 and prints appropiate message.
*/
short checkInputValidity(
    ReturnCode returnCode, 
    char *cannotReadMessage, 
    char *tooLongMessage, 
    char *invalidTypeMessage,
    char *notSupportedMessage
)
{
    switch (returnCode)
    {
        case OK: return (short) 1;
        case ERR_CANNOT_READ:
            if (cannotReadMessage != NULL)
                printf("%s\n", cannotReadMessage);
            break;
        case ERR_TOO_LONG:
            if (tooLongMessage != NULL)
                printf("%s\n", tooLongMessage);
            break;
        case ERR_INVALID_TYPE:
            if (invalidTypeMessage != NULL)
                printf("%s\n", invalidTypeMessage);
            break;
        case ERR_DATATYPE_NOT_SUPPORTED:
            if (notSupportedMessage != NULL)
                printf("%s\n", notSupportedMessage);
            break;
    }

    return (short) 0;
}

/*
    This function clears the console. Works for both Windows and Linux systems.
*/
void clearScreen()
{
    CLSCR();
}