#include "cpa_semantic.h"

CPlusTypeInfo analyse_expression(CPASemanticAnalysis *c, const AST *node)
{
    switch (node->kind) {
        case AST_NUMBER:
            return T(TYPE_INT);
        case AST_STRING:
            return T(TYPE_STRING);
        case AST_IDENT:
            const CPlusTypeInfo *var = _lookup_var(c, node->ident.name);
            __assert(var, "use of undeclared identifier '%.*s' at %zu:%zu", (int) node->ident.name.len, node->ident.name.ptr, node->line,
                node->column);
            return *var;
        case AST_BINOP:
            return analyse_binop(c, &node->binop, node->line, node->column);
        case AST_CALL:
            return analyse_call(c, &node->call, node->line, node->column);
        case AST_EXPRSTMT:
            return analyse_expression(c, node->exprstmt.expr);
        default:
            __assert(false, "Unsupported expression kind %d at %zu:%zu", (int) node->kind, node->line, node->column);
    }
    return T(TYPE_UNKNOWN);
}
