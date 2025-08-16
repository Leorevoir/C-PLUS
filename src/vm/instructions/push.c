#include <error/assert.h>
#include <vm/vm.h>

#include "../vm_helper.h"

void inst_push(VMStack *stack, const int value)
{
    __assert(stack->size < stack->max, "stack overflow");
    stack->memory[stack->size++] = value;
}
