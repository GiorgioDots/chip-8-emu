#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define STACK_CAPACITY 30

typedef uint16_t stack_val_t;

typedef struct
{
    stack_val_t values[STACK_CAPACITY];
    size_t head;
} Stack;

bool pop(Stack *stack, stack_val_t *result);
bool push(Stack *stack, stack_val_t value);

#endif