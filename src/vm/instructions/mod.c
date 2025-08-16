#include <error/assert.h>

#include "../vm_helper.h"

void inst_mod(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    __assert(b != 0, "division by zero");
    inst_push(stack, a % b);
}
