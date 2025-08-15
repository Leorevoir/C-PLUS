#include <error/assert.h>
#include <vm/vm.h>

#include "../vm_helper.h"

int pop(VMStack *stack)
{
    __assert(stack->size > 0, "stack underflow");
    return stack->memory[--stack->size];
}
