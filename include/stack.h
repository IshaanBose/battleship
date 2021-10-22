#define STACK_EMPTY 214424

struct Stack {
    int *stack;
    int size;
    short stackPointer;
};

struct Stack *createStack(int size);
void push(struct Stack *stack, int val);
int pop(struct Stack *stack);
short isStackEmpty(struct Stack *stack);
short stackContains(struct Stack *stack, int value);
void clearStack(struct Stack *stack);
void printStack(struct Stack *stack);
void destoryStack(struct Stack *stackPtr);