#ifndef CPLUS_stack_HELPER_H_
#define CPLUS_stack_HELPER_H_

#include <vm/types.h>

void inst_push(VMStack *stack, const int value);
int inst_pop(VMStack *stack);

void inst_add(VMStack *stack);
void inst_sub(VMStack *stack);
void inst_mul(VMStack *stack);
void inst_div(VMStack *stack);
void inst_mod(VMStack *stack);

#define dispatch_instruction(stack, inst)                                                                              \
    switch ((inst)->type) {                                                                                            \
        case INST_PUSH:                                                                                                \
            inst_push(stack, (inst)->value);                                                                           \
            break;                                                                                                     \
        case INST_POP:                                                                                                 \
            inst_pop(stack);                                                                                           \
            break;                                                                                                     \
        case INST_ADD:                                                                                                 \
            inst_add(stack);                                                                                           \
            break;                                                                                                     \
        case INST_SUB:                                                                                                 \
            inst_sub(stack);                                                                                           \
            break;                                                                                                     \
        case INST_MUL:                                                                                                 \
            inst_mul(stack);                                                                                           \
            break;                                                                                                     \
        case INST_DIV:                                                                                                 \
            inst_div(stack);                                                                                           \
            break;                                                                                                     \
        case INST_MOD:                                                                                                 \
            inst_div(stack);                                                                                           \
            break;                                                                                                     \
        case INST_NEG:                                                                                                 \
            inst_push(stack, -inst_pop(stack));                                                                        \
            break;                                                                                                     \
        default:                                                                                                       \
            break;                                                                                                     \
    }

#endif /* CPLUS_stack_HELPER_H_ */
