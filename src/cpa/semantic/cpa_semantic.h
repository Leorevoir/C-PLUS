#ifndef CPLUS_CPA_SEMANTIC_CPA_SEMANTIC_H_
#define CPLUS_CPA_SEMANTIC_CPA_SEMANTIC_H_

#include "../cpa_utils.h"

#include <cpa/types.h>
#include <oop/map.h>

#include <string.h>

typedef struct {
    Map *map;// map of CPlusTypeInfo
} CPATypeTable;

typedef struct {
    Map *map;// map of CPlusFuncInfo
} CPAFuncTable;

typedef struct {
    CPAFuncTable *funcs;
    CPATypeTable *scopes;
    u64 scope_count;
    u64 scope_cap;
    CPlusTypeInfo current_ret;
} CPASemanticAnalysis;

static __inline CPlusTypeInfo _type_from_ast_type(const AST *t)
{
    if (!t || t->kind != AST_TYPE) {
        return T(TYPE_UNKNOWN);
    }
    if (t->type.name.len == 3 && memcmp(t->type.name.ptr, "int", 3) == 0) {
        return T(TYPE_INT);
    }
    if (t->type.name.len == 5 && memcmp(t->type.name.ptr, "float", 5) == 0) {
        return T(TYPE_FLOAT);
    }
    if (t->type.name.len == 4 && memcmp(t->type.name.ptr, "bool", 4) == 0) {
        return T(TYPE_BOOL);
    }
    if (t->type.name.len == 6 && memcmp(t->type.name.ptr, "string", 6) == 0) {
        return T(TYPE_STRING);
    }
    return T(TYPE_UNKNOWN);
}

static __inline void _init_func_table(CPAFuncTable *out_table)
{
    out_table->map = new (MapClass, 0);
    out_table->map->hash_func = sv_hash;
}

static __inline void _init_checker(CPASemanticAnalysis *checker, CPAFuncTable *funcs)
{
    checker->funcs = funcs;
    checker->scopes = NULL;
    checker->scope_count = 0;
    checker->scope_cap = 0;
    checker->current_ret = T(TYPE_UNKNOWN);
}

static __inline void _init_type_table(CPATypeTable *table)
{
    table->map = new (MapClass, 0);
    table->map->hash_func = sv_hash;
}

static __inline void _push_scope(CPASemanticAnalysis *c)
{
    if (c->scope_count == c->scope_cap) {
        const u64 n = vectorize_size(c->scope_cap);
        reallocate(c->scopes, n * sizeof *c->scopes);
        for (u64 i = 0; i < n; ++i) {
            _init_type_table(&c->scopes[i]);
        }
        c->scope_cap = n;
    }
    _init_type_table(&c->scopes[c->scope_count]);
    ++c->scope_count;
}

static __inline void _pop_scope(CPASemanticAnalysis *c)
{
    __assert(c->scope_count > 0, "scope underflow");
    --c->scope_count;
}

static __inline void _define_type_table(CPASemanticAnalysis *c, StrView name, CPlusTypeInfo type_info, u64 line, u64 column)
{
    __assert(c->scope_count > 0, "no scope to define variable in");

    Map *map = c->scopes[c->scope_count - 1].map;

    if (map->contains(map, &name)) {
        __assert(false, "variable '%.*s' already defined in this scope at line %llu, column %llu", (int) name.len, name.ptr,
            (unsigned long long) line, (unsigned long long) column);
    }

    CPlusTypeInfo *stored;
    allocate(stored, sizeof *stored);
    *stored = type_info;
    stored->name = name;

    map->insert(map, &stored->name, stored);
}

static __inline const CPlusTypeInfo *_lookup_var(CPASemanticAnalysis *c, const StrView name)
{
    for (u64 i = c->scope_count; i > 0; --i) {
        const Map *map = c->scopes[i - 1].map;
        if (map->contains(map, &name)) {
            return (const CPlusTypeInfo *) map->get(map, &name);
        }
    }
    return NULL;
}

void analyse_function(CPASemanticAnalysis *c, const AST *ast);
void analyse_statement(CPASemanticAnalysis *c, const AST *node);

CPlusTypeInfo analyse_expression(CPASemanticAnalysis *c, const AST *expr);
CPlusTypeInfo analyse_binop(CPASemanticAnalysis *c, const ASTBinOp *binop, u64 line, u64 column);
CPlusTypeInfo analyse_call(CPASemanticAnalysis *c, const ASTCall *call, u64 line, u64 column);

#endif /* CPLUS_CPA_SEMANTIC_CPA_SEMANTIC_H_ */
