#include "cpa_semantic.h"

#include <memory/allocate.h>
#include <memory/garbage_collector.h>

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

        CPlusFuncInfo *func_info;

        allocate(func_info, sizeof *func_info);
        func_info->name = func->name;
        func_info->params = params;
        func_info->param_count = paramc;
        func_info->return_type = _type_from_ast_type(func->ret_type);
        funcs->map->insert(funcs->map, &func_info->name, func_info);
    }
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
            analyse_function(&checker, it);
            continue;
        }
        __assert(false, "Unexpected AST node kind in module items, only functions are allowed at module scope for now");
    }
}
