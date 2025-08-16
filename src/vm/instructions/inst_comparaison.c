#include "header.h"

void inst_eq(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a == b ? 1 : 0);
}

void inst_neq(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a != b ? 1 : 0);
}

void inst_gt(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a > b ? 1 : 0);
}
void inst_lt(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a < b ? 1 : 0);
}

void inst_gte(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a >= b ? 1 : 0);
}

void inst_lte(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a <= b ? 1 : 0);
}
