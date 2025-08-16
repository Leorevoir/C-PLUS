#include <error/assert.h>

#include "header.h"

void inst_add(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a + b);
}

void inst_sub(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a - b);
}

void inst_mul(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a * b);
}

void inst_div(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    __assert(b != 0, "division by zero");
    inst_push(stack, a / b);
}

void inst_mod(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    __assert(b != 0, "division by zero");
    inst_push(stack, a % b);
}
