#include <error/assert.h>
#include <vm/vm.h>

#include "header.h"

int inst_pop(VMStack *stack)
{
    __assert(stack->size > 0, "stack underflow");
    return stack->memory[--stack->size];
}

void inst_push(VMStack *stack, const int value)
{
    __assert(stack->size < stack->max, "stack overflow");
    stack->memory[stack->size++] = value;
}
