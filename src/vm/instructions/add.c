#include "../vm_helper.h"

void inst_add(VMStack *stack)
{
    const int b = inst_pop(stack);
    const int a = inst_pop(stack);

    inst_push(stack, a + b);
}
