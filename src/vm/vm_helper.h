#ifndef CPLUS_stack_HELPER_H_
#define CPLUS_stack_HELPER_H_

#include <vm/types.h>

void push(VMStack *stack, const int value);
int pop(VMStack *stack);

#define dispatch_instruction(stack, inst)                                                                              \
    switch ((inst)->type) {                                                                                            \
        case INST_PUSH:                                                                                                \
            push(stack, (inst)->value);                                                                                \
            break;                                                                                                     \
        case INST_POP:                                                                                                 \
            pop(stack);                                                                                                \
            break;                                                                                                     \
        case INST_ADD: {                                                                                               \
            int b = pop(stack);                                                                                        \
            int a = pop(stack);                                                                                        \
            push(stack, a + b);                                                                                        \
        } break;                                                                                                       \
        case INST_SUB: {                                                                                               \
            int b = pop(stack);                                                                                        \
            int a = pop(stack);                                                                                        \
            push(stack, a - b);                                                                                        \
        } break;                                                                                                       \
        case INST_MUL: {                                                                                               \
            int b = pop(stack);                                                                                        \
            int a = pop(stack);                                                                                        \
            push(stack, a * b);                                                                                        \
        } break;                                                                                                       \
        case INST_DIV: {                                                                                               \
            int b = pop(stack);                                                                                        \
            int a = pop(stack);                                                                                        \
            if (b == 0) { /* TODO: handle division by zero */                                                          \
            } else {                                                                                                   \
                push(stack, a / b);                                                                                    \
            }                                                                                                          \
        } break;                                                                                                       \
        case INST_MOD: {                                                                                               \
            int b = pop(stack);                                                                                        \
            int a = pop(stack);                                                                                        \
            if (b == 0) { /* TODO: handle division by zero */                                                          \
            } else {                                                                                                   \
                push(stack, a % b);                                                                                    \
            }                                                                                                          \
        } break;                                                                                                       \
        case INST_NEG:                                                                                                 \
            push(stack, -pop(stack));                                                                                  \
            break;                                                                                                     \
        default:                                                                                                       \
            break;                                                                                                     \
    }

#endif /* CPLUS_stack_HELPER_H_ */
