#include <vm/vm.h>

#include "header.h"

void inst_jmp(VMStack *stack, const int offset)
{
    stack->current += offset;
    __assert(stack->current >= 0, "jump target cannot be negative");
    __assert(stack->current < stack->inst_count, "jump target out of bounds");
}

void inst_jnz(VMStack *stack, const int offset)
{
    const int value = inst_pop(stack);

    if (value != 0) {
        inst_jmp(stack, offset);
    } else {
        ++stack->current;
    }
}

void inst_jz(VMStack *stack, const int offset)
{
    const int value = inst_pop(stack);

    if (value == 0) {
        inst_jmp(stack, offset);
    } else {
        ++stack->current;
    }
}

void inst_call(VMStack *stack, const int target)
{
    __assert(stack->size + 1 < stack->max, "stack overflow");
    stack->memory[stack->size++] = stack->current + 1;
    stack->current = target;
}

void inst_ret(VMStack *stack)
{
    __assert(stack->size > 0, "stack underflow");
    stack->current = stack->memory[--stack->size];
}
