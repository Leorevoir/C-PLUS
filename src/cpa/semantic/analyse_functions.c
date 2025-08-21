#include "cpa_semantic.h"

void analyse_function(CPASemanticAnalysis *c, const AST *node)
{
    const ASTFunc *func = &node->func;
    const CPlusFuncInfo *decl = c->funcs->map->get(c->funcs->map, &func->name);

    __assert(decl, "function must be declared");
    __assert(decl->param_count == func->params.len, "internal error param count mismatch");

    /** set current return type & push a new local scope */
    CPlusTypeInfo prev_ret = c->current_ret;
    c->current_ret = decl->return_type;
    _push_scope(c);

    /** bind parameters in the new top scope */
    for (u64 i = 0; i < func->params.len; ++i) {
        const AST *params = func->params.data[i];
        __assert(params->kind == AST_PARAM, "Expected AST_PARAM in function parameters");
        const CPlusTypeInfo type_info = _type_from_ast_type(params->param.type);
        _define_type_table(c, params->param.name, type_info, params->line, params->column);
    }

    /** check body */
    __assert(func->body && func->body->kind == AST_BLOCK, "Function body must be a block");
    analyse_statement(c, func->body);

    /** pop scope & restore previous return type */
    _pop_scope(c);
    c->current_ret = prev_ret;
}
