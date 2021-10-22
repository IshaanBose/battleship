#include <stack.h>
#include <stdlib.h>
#include <stdio.h>

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

int pop(struct Stack *stack)
{
    if (!isStackEmpty(stack))
    {
        return stack->stack[--stack->stackPointer];
    }

    return STACK_EMPTY;
}

short isStackEmpty(struct Stack *stack)
{
    return (stack->stackPointer == 0);
}

short stackContains(struct Stack *stack, int value)
{
    for (int i = 0; i < stack->stackPointer; i++)
        if (stack->stack[i] == value)
            return 1;

    return 0;
}

void clearStack(struct Stack *stack)
{
    stack->stackPointer = 0;
}

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

void destoryStack(struct Stack *stackPtr)
{
    free(stackPtr->stack);
    free(stackPtr);
}