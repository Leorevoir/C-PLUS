#ifndef CPLUS_PARSER_SHOW_H_
#define CPLUS_PARSER_SHOW_H_

#include <cplus/parser.h>
#include <error/assert.h>
#include <stdio.h>

void parser_show(const Parser *self);

/**
 * dump helpers
 */

static __inline void sv_dump(const StrView s)
{
    for (u64 i = 0; i < s.len; ++i)
        putchar(s.ptr[i]);
}

static void ast_dump(const AST *n, int indent)
{
    if (!n) {
        printf("%*s(null)\n", (int) indent, "");
        return;
    }

    const char *pad = "                                                                ";
    switch (n->kind) {
        case AST_MODULE:
            printf("%.*sModule\n", indent, pad);
            for (size_t i = 0; i < n->module.items.len; ++i)
                ast_dump(n->module.items.data[i], indent + 2);
            break;
        case AST_FUNC:
            printf("%.*sFunc ", indent, pad);
            sv_dump(n->func.name);
            printf("\n");
            printf("%.*s  params:\n", indent, pad);
            for (size_t i = 0; i < n->func.params.len; ++i)
                ast_dump(n->func.params.data[i], indent + 4);
            printf("%.*s  ret:\n", indent, pad);
            ast_dump(n->func.ret_type, indent + 4);
            printf("%.*s  body:\n", indent, pad);
            ast_dump(n->func.body, indent + 4);
            break;
        case AST_PARAM:
            printf("%.*sParam ", indent, pad);
            sv_dump(n->param.name);
            printf(" : ");
            ast_dump(n->param.type, 0);
            break;
        case AST_TYPE:
            printf("%.*sType ", indent, pad);
            sv_dump(n->type.name);
            printf("\n");
            break;
        case AST_BLOCK:
            printf("%.*sBlock\n", indent, pad);
            for (size_t i = 0; i < n->block.items.len; ++i)
                ast_dump(n->block.items.data[i], indent + 2);
            break;
        case AST_RETURN:
            printf("%.*sReturn\n", indent, pad);
            ast_dump(n->ret.value, indent + 2);
            break;
        case AST_LET:
            printf("%.*sLet ", indent, pad);
            sv_dump(n->let.name);
            printf(" =\n");
            ast_dump(n->let.value, indent + 2);
            break;
        case AST_CALL:
            printf("%.*sCall ", indent, pad);
            sv_dump(n->call.name);
            printf("(\n");
            for (size_t i = 0; i < n->call.args.len; ++i)
                ast_dump(n->call.args.data[i], indent + 2);
            printf("%.*s)\n", indent, pad);
            break;
        case AST_BINOP:
            printf("%.*sBinOp '", indent, pad);
            sv_dump(n->binop.op);
            printf("'\n");
            ast_dump(n->binop.left, indent + 2);
            ast_dump(n->binop.right, indent + 2);
            break;
        case AST_UNOP:
            printf("%.*sUnOp '", indent, pad);
            sv_dump(n->unop.op);
            printf("'\n");
            ast_dump(n->unop.operand, indent + 2);
            break;
        case AST_IDENT:
            printf("%.*sIdent ", indent, pad);
            sv_dump(n->ident.name);
            printf("\n");
            break;
        case AST_NUMBER:
            printf("%.*sNumber ", indent, pad);
            sv_dump(n->number.value);
            printf("\n");
            break;
        case AST_STRING:
            printf("%.*sString ", indent, pad);
            sv_dump(n->string.value);
            printf("\n");
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
