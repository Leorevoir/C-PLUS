#include <vm/vm.h>

#include "header.h"

void inst_halt(VMStack *stack)
{
    stack->current = stack->inst_count;
}
