#include "cpa_semantic.h"

CPlusTypeInfo analyse_call(CPASemanticAnalysis *c, const ASTCall *call, u64 line, u64 column)
{
    const CPlusFuncInfo *func = c->funcs->map->get(c->funcs->map, &call->name);

    __assert(func, "Undefined function call `%.*s` at %zu:%zu", (int) call->name.len, call->name.ptr, line, column);
    __assert(call->args.len == func->param_count, "Function `%.*s` expects %zu argument(s), got %zu at %zu:%zu", (int) call->name.len,
        call->name.ptr, func->param_count, call->args.len, line, column);

    const u64 n = (call->args.len < func->param_count) ? call->args.len : func->param_count;

    for (u64 i = 0; i < n; ++i) {
        const CPlusTypeInfo got = analyse_expression(c, call->args.data[i]);
        const CPlusTypeInfo expected = func->params[i];

        __assert(cplus_type_eq(got, expected), "Argument %llu type mismatch: expected %s, got %s at %zu:%zu", (unsigned long long) (i + 1),
            cplus_type_name(expected), cplus_type_name(got), line, column);
    }

    return func->return_type;
}
