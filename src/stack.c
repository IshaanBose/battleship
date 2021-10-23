/*

File Name: stack.c
Author: Ishaan Bose
Date of Creation: 2021-10-21
____________________________________________________________________________________________________________________________________ 

    Contains functions to dynamically create a stack and perform operations on it.

    INCLUDES (User-defined)
    -----------------------
    1. stack.h

    FUNCTIONS (global)
    ----------------
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

#include <stack.h>
#include <stdlib.h>
#include <stdio.h>

/*
    Creates a stack of given size. The stack can only contain integers.

    Parameter
    ---------
    `int size`:
        Specifies the size of the stack.
    
    Returns
    -------
    Returns a pointer to the created Stack struct.
*/
struct Stack *createStack(int size)
{
    struct Stack *stackPtr = (struct Stack*) malloc(sizeof(struct Stack));

    if (stackPtr == NULL)
        return NULL;
    
    stackPtr->size = size;
    stackPtr->stackPointer = 0;

    stackPtr->stack = (int *) malloc(size * sizeof(int));

    if (stackPtr->stack == NULL)
        return NULL;

    return stackPtr;
}

/*
    Pushes an integer values on top of the given stack.

    Parameters
    ----------
    `struct Stack *stack`:
        Pointer to the stack to push the value into.
    
    `int val`:
        Value to push into stack.
*/
void push(struct Stack *stack, int val)
{
    if ((stack->stackPointer - 1) != stack->size)
    {
        stack->stack[stack->stackPointer] = val;
        stack->stackPointer += 1;
    }
    else
    {
        printf("ERROR: Stack is full.\n");
    }
}

/*
    Returns and removes the element from the top of the given stack.

    Parameter
    ---------
    `struct Stack *stack`:
        Pointer to the stack from which to pop the value from.
    
    Returns
    -------
    Returns the topmost value from the given stack, if the stack if empty, returns STACK_EMPTY
*/
int pop(struct Stack *stack)
{
    if (!isStackEmpty(stack))
    {
        return stack->stack[--stack->stackPointer];
    }

    return STACK_EMPTY;
}

/*
    Checks whether the given stack is empty or not.

    Parameter
    ---------
    `struct Stack *stack`:
        Pointer to the stack which needs to be checked.
    
    Returns
    -------
    Returns 0 if stack is not empty, else it returns 1.
*/
short isStackEmpty(struct Stack *stack)
{
    return (stack->stackPointer == 0);
}

/*
    Checks whether the given stack contains the given value.

    Parameter
    ---------
    `struct Stack *stack`:
        Pointer to the stack which needs to be checked.
    
    `int value`:
        The value to check the stack for.
    
    Returns
    -------
    Returns 0 if the element is not present in the stack, else it returns 1.
*/
short stackContains(struct Stack *stack, int value)
{
    for (int i = 0; i < stack->stackPointer; i++)
        if (stack->stack[i] == value)
            return 1;

    return 0;
}

/*
    Empties the given stack.

    Parameter
    ---------
    `struct Stack *stack`:
        Pointer to the stack which needs to be emptied.
*/
void clearStack(struct Stack *stack)
{
    stack->stackPointer = 0;
}

/*
    Prints the given stack to stdout.

    `struct Stack *stack`:
        Pointer to the stack which needs to be printed.
*/
void printStack(struct Stack *stack)
{
    printf("stack pointer: %d\n\n", stack->stackPointer);
    for (int i = 0; i < stack->stackPointer; i++)
    {
        if (i == 0)
            printf("| %d | ", stack->stack[i]);
        else if (i == stack->stackPointer - 1)
            printf("%d\n", stack->stack[i]);
        else
            printf("%d | ", stack->stack[i]);
    }
}

/*
    Deallocates memory from the given stack.

    Parameter
    ---------
    `struct Stack *stack`:
        Pointer to the stack which needs have its memory deallocate.
*/
void destoryStack(struct Stack *stackPtr)
{
    free(stackPtr->stack);
    free(stackPtr);
}