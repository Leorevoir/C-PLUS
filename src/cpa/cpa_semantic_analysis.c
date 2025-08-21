#include "cpa/types.h"
#include "cpa_utils.h"
#include "memory/allocate.h"
#include "memory/garbage_collector.h"
#include "oop/map.h"
#include <string.h>

typedef struct {
    Map *map;// map of CPlusTypeInfo
} CPAVarTable;

typedef struct {
    Map *map;// map of CPlusFuncInfo
} CPAFuncTable;

typedef struct {
    CPAFuncTable *funcs;
    CPAVarTable *scopes;
    u64 scope_count;
    u64 scope_cap;
    CPlusTypeInfo current_ret;
} CPASemanticAnalysis;

static __inline CPlusTypeInfo T(const CPlusTypeKind kind)
{
    CPlusTypeInfo type_info;

    type_info.name.ptr = NULL;
    type_info.name.len = 0;
    type_info.kind = kind;
    return type_info;
}

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
static void cpassembly_collect_function_declarations(CPAFuncTable *funcs, AST *root)
{
    for (u64 i = 0; i < root->module.items.len; ++i) {
        const AST *it = root->module.items.data[i];

        if (it->kind != AST_FUNC) {
            continue;
        }

        const ASTFunc *func = &it->func;
        u64 paramc = func->params.len;
        CPlusTypeInfo *params = NULL;

        if (paramc) {
            reallocate(params, paramc * sizeof *params);
            for (u64 p = 0; p < paramc; ++p) {
                const AST *pa = func->params.data[p];
                __assert(pa->kind == AST_PARAM, "Expected AST_PARAM in function parameters");
                const CPlusTypeInfo type_info = _type_from_ast_type(pa->param.type);
                params[p] = type_info;
            }
        }

        CPlusFuncInfo func_info;
        func_info.name = func->name;
        func_info.params = params;
        func_info.param_count = paramc;
        func_info.return_type = _type_from_ast_type(func->ret_type);
        funcs->map->insert(funcs->map, &func_info.name, &func_info);
    }
}

static void cpassembly_check_function(CPASemanticAnalysis *c, const AST *node)
{
    const ASTFunc *func = &node->func;
    const CPlusFuncInfo *decl = c->funcs->map->get(c->funcs->map, &func->name);

    __assert(decl, "function must be declared");
}

void cpassembly_semantic_analysis(AST *root)
{
    __assert(root && root->kind == AST_MODULE, "Root AST must be a module");

    CPAFuncTable funcs;
    _init_func_table(&funcs);
    cpassembly_collect_function_declarations(&funcs, root);

    CPASemanticAnalysis checker;
    _init_checker(&checker, &funcs);

    for (u64 i = 0; i < root->module.items.len; ++i) {
        const AST *it = root->module.items.data[i];

        if (it->kind == AST_FUNC) {
            cpassembly_check_function(&checker, it);
            continue;
        }
        __assert(false, "Unexpected AST node kind in module items, only functions are allowed at module scope for now");
    }
}
