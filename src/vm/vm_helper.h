#ifndef CPLUS_stack_HELPER_H_
#define CPLUS_stack_HELPER_H_

#include "instructions/header.h"

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
        case INST_EQ:                                                                                                  \
            inst_eq(stack);                                                                                            \
            break;                                                                                                     \
        case INST_NEQ:                                                                                                 \
            inst_neq(stack);                                                                                           \
            break;                                                                                                     \
        case INST_LT:                                                                                                  \
            inst_lt(stack);                                                                                            \
            break;                                                                                                     \
        case INST_GT:                                                                                                  \
            inst_gt(stack);                                                                                            \
            break;                                                                                                     \
        case INST_LTE:                                                                                                 \
            inst_lte(stack);                                                                                           \
            break;                                                                                                     \
        case INST_GTE:                                                                                                 \
            inst_gte(stack);                                                                                           \
            break;                                                                                                     \
        case INST_JMP:                                                                                                 \
            inst_jmp(stack, (inst)->value);                                                                            \
            break;                                                                                                     \
        case INST_JZ:                                                                                                  \
            inst_jz(stack, (inst)->value);                                                                             \
            break;                                                                                                     \
        case INST_JNZ:                                                                                                 \
            inst_jnz(stack, (inst)->value);                                                                            \
            break;                                                                                                     \
        case INST_CALL:                                                                                                \
            inst_call(stack, (inst)->value);                                                                           \
            break;                                                                                                     \
        case INST_RET:                                                                                                 \
            inst_ret(stack);                                                                                           \
            break;                                                                                                     \
        case INST_HALT:                                                                                                \
            inst_halt(stack);                                                                                          \
            break;                                                                                                     \
        default:                                                                                                       \
            break;                                                                                                     \
    }

#endif /* CPLUS_stack_HELPER_H_ */
