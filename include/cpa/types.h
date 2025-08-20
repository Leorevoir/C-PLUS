#ifndef CPLUS_CPA_TYPES_H_
#define CPLUS_CPA_TYPES_H_

#include <cplus/types.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_UNKNOWN,
} CPlusTypeKind;

typedef struct {
    CPlusTypeKind kind;
} CPlusTypeInfo;

static __inline CPlusTypeInfo cplus_type_info(const CPlusTypeKind k)
{
    return (const CPlusTypeInfo) {
        .kind = k,
    };
}

typedef enum {

    /** arithmetic */
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_MOD,
    INST_INC,
    INST_DEC,
    INST_NEG,

    /** bitwise */
    INST_AND,
    INST_OR,
    INST_XOR,
    INST_NOT,

    /** comparison */
    INST_EQ,
    INST_NEQ,
    INST_LT,
    INST_GT,
    INST_LTE,
    INST_GTE,
    INST_CMP,

    /** control flow */
    INST_JUMP,
    INST_JUMP_IF,
    INST_JUMP_IF_NOT,
    INST_CALL,

    INST_MOV,
    INST_MOV_IMM,

    INST_RET,
    INST_HALT,
} CPlusInstTypeKind;

typedef struct {
    CPlusInstTypeKind type;
    u8 dest;
    u8 src[2];
    u8 imm;
} CPlusInst;

#endif /* CPLUS_CPA_TYPES_H_ */
