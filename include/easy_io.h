/*

File Name: easy_io.h
Author: Ishaan Bose
Date of Creation: 2021-09-24
____________________________________________________________________________________________________________________________________ 

    Header file for easy_io.c.

    INCLUDES
    --------
    stdio.h

    ENUMS
    -----
    1. DataType
    2. ReturnCode

    FUNCTIONS
    ---------
    1. ReturnCode takeInput(void *inputVar, DataType type)
    2. ReturnCode takeNInput(void *inputVar, DataType type, int inputSize)
    3. ReturnCode takeInputWithPropmt(char *propmt, void *inputVar, DataType type)
    4. ReturnCode takeNInputWithPropmt(char *propmt, void *inputVar, DataType type, int inputSize)
    5. void clearInputBuffer(void)
    6. void clearScreen(void)
    7. short checkInputValidity(
            ReturnCode returnCode, 
            char *cannotReadMessage, 
            char *tooLongMessage, 
            char *invalidTypeMessage, 
            char *notSupportedMessage
        )
*/

#include <stdio.h>

// enum containing all the data types that are supported by the takeInput functions.
typedef enum {
    INT, 
    CHAR, 
    STR_WHITESPC,
    FLOAT, 
    SHORT, 
    STRING
} DataType;

// enum containing all values that are returned by the takeInput functions.
typedef enum {
    OK = 1,
    ERR_CANNOT_READ = -1,
    ERR_TOO_LONG = -2,
    ERR_INVALID_TYPE = -3,
    ERR_DATATYPE_NOT_SUPPORTED = -4
} ReturnCode;

// Functions

ReturnCode takeInput(void *inputVar, DataType type);
ReturnCode takeNInput(void *inputVar, DataType type, int inputSize);
ReturnCode takeInputWithPropmt(char *propmt, void *inputVar, DataType type);
ReturnCode takeNInputWithPropmt(char *propmt, void *inputVar, DataType type, int inputSize);
void clearInputBuffer(void);
void clearScreen(void);
short checkInputValidity(
    ReturnCode returnCode, 
    char *cannotReadMessage, 
    char *tooLongMessage, 
    char *invalidTypeMessage,
    char *notSupportedMessage
);