#include "cpa_semantic.h"

static __inline void _analyse_let(CPASemanticAnalysis *c, const AST *node)
{
    const CPlusTypeInfo type = analyse_expression(c, node->let.value);

    _define_type_table(c, node->let.name, type, node->line, node->column);
}

static __inline void _analyse_return(CPASemanticAnalysis *c, const AST *node)
{
    const CPlusTypeInfo ret_type = node->ret.value ? analyse_expression(c, node->ret.value) : T(TYPE_NULL);

    __assert(cplus_type_eq(ret_type, c->current_ret), "return type mismatch: function returns %s but got %s at %zu:%zu",
        cplus_type_name(c->current_ret), cplus_type_name(ret_type), node->line, node->column);
}

static __inline void _analyse_block(CPASemanticAnalysis *c, const AST *node)
{
    _push_scope(c);
    for (u64 i = 0; i < node->block.items.len; ++i) {
        analyse_statement(c, node->block.items.data[i]);
    }
    _pop_scope(c);
}

void analyse_statement(CPASemanticAnalysis *c, const AST *node)
{
    switch (node->kind) {
        case AST_LET:
            _analyse_let(c, node);
            break;
        case AST_RETURN:
            _analyse_return(c, node);
            break;
        case AST_EXPRSTMT:
            (void) analyse_expression(c, node->exprstmt.expr);
            break;
        case AST_BLOCK:
            _analyse_block(c, node);
            break;
        default:
            __assert(false, "Unsupported statement kind %d at %zu:%zu", (int) node->kind, node->line, node->column);
    }
}
