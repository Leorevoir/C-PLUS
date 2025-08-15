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

#define err_to_str(err)                                                                                                \
    ((err) == ERR_OK                                  ? "No error"                                                     \
            : (err) == ERR_INVALID_ARGUMENT           ? "Invalid argument"                                             \
            : (err) == ERR_STACK_UNDERFLOW            ? "Stack underflow"                                              \
            : (err) == ERR_STACK_OVERFLOW             ? "Stack overflow"                                               \
            : (err) == ERR_OUT_OF_MEMORY              ? "Out of memory"                                                \
            : (err) == ERR_NULL_POINTER               ? "Null pointer"                                                 \
            : (err) == ERR_DIV_BY_ZERO                ? "Division by zero"                                             \
            : (err) == ERR_ILLEGAL_INSTRUCTION        ? "Illegal instruction"                                          \
            : (err) == ERR_ILLEGAL_INSTRUCTION_ACCESS ? "Illegal instruction access"                                   \
            : (err) == ERR_ILLEGAL_MEMORY_ACCESS      ? "Illegal memory access"                                        \
            : (err) == ERR_ILLEGAL_OPERAND            ? "Illegal operand"                                              \
                                                      : "Unknown error")
#endif /* CPLUS_VM_TYPES_H_ */
