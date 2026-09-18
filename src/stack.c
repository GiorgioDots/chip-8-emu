#include "stack.h"

bool pop(Stack *stack, stack_val_t *result)
{
    if (stack == NULL || result == NULL || stack->head == 0)
    {
        return false;
    }

    stack->head--;
    *result = stack->values[stack->head];

    return true;
}

bool push(Stack *stack, stack_val_t value)
{
    if (stack == NULL || stack->head >= STACK_CAPACITY)
    {
        return false;
    }

    stack->values[stack->head] = value;
    stack->head++;

    return true;
}