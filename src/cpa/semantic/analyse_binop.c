#include "cpa_semantic.h"

static CPlusTypeInfo _binop_add_or_concat(const CPlusTypeInfo left, const CPlusTypeInfo right, u64 line, u64 column)
{
    if (left.kind == TYPE_INT && right.kind == TYPE_INT) {
        return T(TYPE_INT);
    }
    if (left.kind == TYPE_FLOAT && right.kind == TYPE_FLOAT) {
        return T(TYPE_FLOAT);
    }
    if (left.kind == TYPE_STRING || right.kind == TYPE_STRING) {
        return T(TYPE_STRING);
    }
    __assert(false, "Unsupported binary operation `%c` at %zu:%zu", '+', line, column);
    return T(TYPE_UNKNOWN);//forced to put this to avoid stupid compiler warning......... even if __assert() throw an exception....
}

CPlusTypeInfo analyse_binop(CPASemanticAnalysis *c, const ASTBinOp *binop, u64 line, u64 column)
{
    const CPlusTypeInfo left = analyse_expression(c, binop->left);
    const CPlusTypeInfo right = analyse_expression(c, binop->right);

    __assert(binop->op.len == 1, "Only single-character operators are supported in this version");
    switch (binop->op.ptr[0]) {
        case '+':
            return _binop_add_or_concat(left, right, line, column);
        default:
            __assert(false, "Unsupported binary operator `%c` at %zu:%zu", binop->op.ptr[0], line, column);
    }
    return T(TYPE_UNKNOWN);//forced to put this to avoid stupid compiler warning......... even if __assert() throw an exception....
}
