/*

File Name: stack.c
Author: Ishaan Bose
Date of Creation: 2021-10-21
____________________________________________________________________________________________________________________________________ 

    Header file for stack.c.

    MACRO
    -----
    STACK_EMPTY

    STRUCT
    ------
    Stack

    FUNCTIONS
    ---------
    1. struct Stack *createStack(int size)
    2. void push(struct Stack *stack, int val)
    3. int pop(struct Stack *stack)
    4. short isStackEmpty(struct Stack *stack)
    5. short stackContains(struct Stack *stack, int value)
    6. void clearStack(struct Stack *stack)
    7. void printStack(struct Stack *stack)
    8. void destoryStack(struct Stack *stackPtr)

*Compiled using C99 standards*

*/

// macros
#define STACK_EMPTY 214424

/*
    Stack struct, contains:
    - int *stack: pointer to array which is to be treated as a stack
    - int size: size of stack
    - short stackPointer: points to the top of the stack, i.e., where to push to, and pop elements from.
*/
struct Stack {
    int *stack;
    int size;
    short stackPointer;
};

// functions

struct Stack *createStack(int size);
void push(struct Stack *stack, int val);
int pop(struct Stack *stack);
short isStackEmpty(struct Stack *stack);
short stackContains(struct Stack *stack, int value);
void clearStack(struct Stack *stack);
void printStack(struct Stack *stack);
void destoryStack(struct Stack *stackPtr);