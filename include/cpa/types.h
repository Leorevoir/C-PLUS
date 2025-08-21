#ifndef CPLUS_CPA_TYPES_H_
#define CPLUS_CPA_TYPES_H_

#include <cplus/types.h>
#include <oop/string.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_NULL,
    TYPE_UNKNOWN,
} CPlusTypeKind;

typedef struct {
    StrView name;
    CPlusTypeKind kind;
} CPlusTypeInfo;

typedef struct {
    StrView name;
    CPlusTypeInfo *params;
    u64 param_count;
    CPlusTypeInfo return_type;
} CPlusFuncInfo;

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

static __inline CPlusTypeInfo T(const CPlusTypeKind kind)
{
    CPlusTypeInfo type_info;

    type_info.name.ptr = NULL;
    type_info.name.len = 0;
    type_info.kind = kind;
    return type_info;
}

__cplus__const static __inline const char *cplus_type_name(const CPlusTypeInfo type_info)
{
    switch (type_info.kind) {
        case TYPE_INT:
            return "int";
        case TYPE_FLOAT:
            return "float";
        case TYPE_STRING:
            return "string";
        case TYPE_BOOL:
            return "bool";
        case TYPE_NULL:
            return "null";
        default:
            return "<unknown>";
    }
}

static __inline bool cplus_type_eq(const CPlusTypeInfo a, const CPlusTypeInfo b)
{
    return (a.kind == b.kind);
}

#endif /* CPLUS_CPA_TYPES_H_ */
