#ifndef CPLUS_VM_TYPES_H_
#define CPLUS_VM_TYPES_H_

#include <stddef.h>

typedef enum {
    ERR_OK = 0,
    ERR_INVALID_ARGUMENT,
    ERR_STACK_UNDERFLOW,
    ERR_STACK_OVERFLOW,
    ERR_OUT_OF_MEMORY,
    ERR_NULL_POINTER,
    ERR_DIV_BY_ZERO,
    ERR_ILLEGAL_INSTRUCTION,
    ERR_ILLEGAL_INSTRUCTION_ACCESS,
    ERR_ILLEGAL_MEMORY_ACCESS,
    ERR_ILLEGAL_OPERAND,
} Err;

typedef enum {
    INST_PUSH,
    INST_POP,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_MOD,
    INST_NEG,
    INST_EQ,
    INST_NEQ,
    INST_LT,
    INST_GT,
    INST_LTE,
    INST_GTE,
    INST_JMP,
    INST_JZ,
    INST_JNZ,
    INST_CALL,
    INST_RET,
    INST_HALT,
} InstType;

typedef enum {
    TYPE_ANY,
    TYPE_INT,
    TYPE_UINT,
    TYPE_FLOAT,
    TYPE_BOOL,
} Type;

typedef struct {
    InstType type;
    int value;
} Inst;

__attribute__((const)) static inline const char *inst_to_str(const Inst *inst)
{
    switch (inst->type) {
        case INST_PUSH:
            return "PUSH";
        case INST_POP:
            return "POP";
        case INST_ADD:
            return "ADD";
        case INST_SUB:
            return "SUB";
        case INST_MUL:
            return "MUL";
        case INST_DIV:
            return "DIV";
        case INST_MOD:
            return "MOD";
        case INST_NEG:
            return "NEG";
        case INST_EQ:
            return "EQ";
        case INST_NEQ:
            return "NEQ";
        case INST_LT:
            return "LT";
        case INST_GT:
            return "GT";
        case INST_LTE:
            return "LTE";
        case INST_GTE:
            return "GTE";
        case INST_JMP:
            return "JMP";
        case INST_JZ:
            return "JZ";
        case INST_JNZ:
            return "JNZ";
        case INST_CALL:
            return "CALL";
        case INST_RET:
            return "RET";
        case INST_HALT:
            return "HALT";
        default:
            return "UNKNOWN";
    }
}

#endif /* CPLUS_VM_TYPES_H_ */
