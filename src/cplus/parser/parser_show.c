#ifndef CPLUS_PARSER_SHOW_H_
#define CPLUS_PARSER_SHOW_H_

#include <cplus/parser.h>
#include <error/assert.h>
#include <stdio.h>

__cplus__used void parser_show(const Parser *self);
__cplus__used static void ast_dump(const AST *n, int indent);

/**
 * dump helpers
 */

static const char *pad = "                                                                ";

static __inline void sv_dump(const StrView s)
{
    for (u64 i = 0; i < s.len; ++i)
        putchar(s.ptr[i]);
}

static __inline void ast_module_dump(const ASTModule *m, int indent)
{
    printf("%.*sModule \"", indent, pad);
    sv_dump(m->name);
    printf("\"\n");
    for (size_t i = 0; i < m->items.len; ++i) {
        ast_dump(m->items.data[i], indent + 2);
    }
}

static __inline void ast_func_dump(const ASTFunc *f, int indent)
{
    printf("%.*sFunc ", indent, pad);
    sv_dump(f->name);

    printf("\n%.*s  params:\n", indent, pad);
    for (u64 i = 0; i < f->params.len; ++i) {
        ast_dump(f->params.data[i], indent + 4);
    }

    printf("%.*s  ret:\n", indent, pad);
    ast_dump(f->ret_type, indent + 4);
    printf("%.*s  body:\n", indent, pad);
    ast_dump(f->body, indent + 4);
}

static __inline void ast_param_dump(const ASTParam *p, int indent)
{
    printf("%.*sParam ", indent, pad);
    sv_dump(p->name);
    printf(" : ");
    ast_dump(p->type, 0);
}

static __inline void ast_type_dump(const ASTType *t, int indent)
{
    printf("%.*sType ", indent, pad);
    sv_dump(t->name);
    printf("\n");
}

static __inline void ast_block_dump(const ASTBlock *b, int indent)
{
    printf("%.*sBlock\n", indent, pad);
    for (size_t i = 0; i < b->items.len; ++i) {
        ast_dump(b->items.data[i], indent + 2);
    }
}

static __inline void ast_return_dump(const ASTReturn *r, int indent)
{
    printf("%.*sReturn\n", indent, pad);
    ast_dump(r->value, indent + 2);
}

static __inline void ast_let_dump(const ASTLet *l, int indent)
{
    printf("%.*sLet ", indent, pad);
    sv_dump(l->name);
    printf(" =\n");
    ast_dump(l->value, indent + 2);
}

static __inline void ast_call_dump(const ASTCall *c, int indent)
{
    printf("%.*sCall ", indent, pad);
    sv_dump(c->name);
    printf("(\n");
    for (size_t i = 0; i < c->args.len; ++i) {
        ast_dump(c->args.data[i], indent + 2);
    }
    printf("%.*s)\n", indent, pad);
}

static __inline void ast_binop_dump(const ASTBinOp *b, int indent)
{
    printf("%.*sBinOp '", indent, pad);
    sv_dump(b->op);
    printf("'\n");
    ast_dump(b->left, indent + 2);
    ast_dump(b->right, indent + 2);
}

static __inline void ast_unop_dump(const ASTUnOp *u, int indent)
{
    printf("%.*sUnOp '", indent, pad);
    sv_dump(u->op);
    printf("'\n");
    ast_dump(u->operand, indent + 2);
}

static __inline void ast_ident_dump(const ASTIdent *i, int indent)
{
    printf("%.*sIdent ", indent, pad);
    sv_dump(i->name);
    printf("\n");
}

static __inline void ast_number_dump(const ASTNumber *n, int indent)
{
    printf("%.*sNumber ", indent, pad);
    sv_dump(n->value);
    printf("\n");
}

static __inline void ast_string_dump(const ASTString *s, int indent)
{
    printf("%.*sString ", indent, pad);
    sv_dump(s->value);
    printf("\n");
}

static __inline void ast_exprstmt_dump(const ASTExprStmt *e, int indent)
{
    printf("%.*sExprStmt\n", indent, pad);
    ast_dump(e->expr, indent + 2);
}

static void ast_dump(const AST *n, int indent)
{
    if (!n) {
        printf("%*s(null)\n", (int) indent, "");
        return;
    }

    switch (n->kind) {

        case AST_MODULE:
            ast_module_dump(&n->module, indent);
            break;

        case AST_FUNC:
            ast_func_dump(&n->func, indent);
            break;

        case AST_PARAM:
            ast_param_dump(&n->param, indent);
            break;

        case AST_TYPE:
            ast_type_dump(&n->type, indent);
            break;

        case AST_BLOCK:
            ast_block_dump(&n->block, indent);
            break;

        case AST_RETURN:
            ast_return_dump(&n->ret, indent);
            break;

        case AST_LET:
            ast_let_dump(&n->let, indent);
            break;

        case AST_CALL:
            ast_call_dump(&n->call, indent);
            break;

        case AST_BINOP:
            ast_binop_dump(&n->binop, indent);
            break;

        case AST_UNOP:
            ast_unop_dump(&n->unop, indent);
            break;

        case AST_IDENT:
            ast_ident_dump(&n->ident, indent);
            break;

        case AST_NUMBER:
            ast_number_dump(&n->number, indent);
            break;

        case AST_STRING:
            ast_string_dump(&n->string, indent);
            break;

        case AST_EXPRSTMT:
            ast_exprstmt_dump(&n->exprstmt, indent);
            break;

        default:
            __assert(false, "Unknown AST kind: %d", n->kind);
    }
}

/**
 * ast show entry point
 */

void __cplus__used parser_show(const Parser *self)
{
    ast_dump(self->root, 0);
}

#endif /* CPLUS_PARSER_SHOW_H_ */
