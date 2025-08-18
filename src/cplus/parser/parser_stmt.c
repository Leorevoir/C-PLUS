#include "parser_utils.h"

#include <string.h>

AST *parse_expression(Parser *p);
AST *parse_module(Parser *p);
static AST *parse_statement(Parser *p);

/**
 * helpers
 */

static AST *parse_type(Parser *p)
{
    const CPlusToken *t = peek(p);

    if (!t || t->kind != CPLUS_TOKEN_KIND_NAME) {
        p->error = (typeof(p->error)) {.panic = true, .msg = "expected type name", .line = t ? t->line : 0, .column = t ? t->column : 0};
        return NULL;
    }
    advance(p);

    AST *ty = node_new(AST_TYPE, t->line, t->column);
    ty->type.name = sv_from(p, t);
    return ty;
}

static AST *parse_param(Parser *p)
{
    const CPlusToken *name = peek(p);

    if (!name || name->kind != CPLUS_TOKEN_KIND_NAME) {
        return NULL;
    }
    advance(p);
    expect(p, CPLUS_TOKEN_KIND_COLON, "expected ':' in parameter");

    AST *ty = parse_type(p);
    AST *par = node_new(AST_PARAM, name->line, name->column);

    par->param.name = sv_from(p, name);
    par->param.type = ty;
    return par;
}

static void parse_param_list(Parser *p, ASTList *out)
{
    const CPlusToken *t = peek(p);

    if (t && t->kind == CPLUS_TOKEN_KIND_CLOSE_PAREN) {
        return;
    }
    for (;;) {
        AST *pa = parse_param(p);

        if (!pa) {
            p->error =
                (typeof(p->error)) {.panic = true, .msg = "expected parameter", .line = t ? t->line : 0, .column = t ? t->column : 0};
            break;
        }
        ast_list_push(out, pa);
        if (!match(p, CPLUS_TOKEN_KIND_COMMA)) {
            break;
        }
    }
}

static AST *parse_block(Parser *p)
{
    const CPlusToken *lb = peek(p);

    expect(p, CPLUS_TOKEN_KIND_OPEN_CURLY, "expected '{'");
    if (p->error.panic) {
        return NULL;
    }

    AST *blk = node_new(AST_BLOCK, lb ? lb->line : 0, lb ? lb->column : 0);

    for (;;) {
        const CPlusToken *t = peek(p);

        if (!t || t->kind == CPLUS_TOKEN_KIND_CLOSE_CURLY) {
            break;
        }

        AST *st = parse_statement(p);

        if (st) {
            ast_list_push(&blk->block.items, st);
        }
        if (p->error.panic) {
            break;
        }
    }
    expect(p, CPLUS_TOKEN_KIND_CLOSE_CURLY, "expected '}'");
    return blk;
}

static AST *parse_statement(Parser *p)
{
    const CPlusToken *t = peek(p);

    if (!t) {
        return NULL;
    }

    if (t->kind == CPLUS_TOKEN_KIND_NAME) {
        const StrView sv = sv_from(p, t);

        if (sv.len == 6 && strncmp(sv.ptr, "return", 6) == 0) {
            advance(p);
            AST *e = parse_expression(p);
            expect(p, CPLUS_TOKEN_KIND_SEMICOLON, "expected ';' after return");
            AST *r = node_new(AST_RETURN, t->line, t->column);
            r->ret.value = e;
            return r;
        }
    }

    if (t->kind == CPLUS_TOKEN_KIND_NAME) {
        const CPlusToken *name = t;
        const CPlusToken *t1 = (p->cursor + 1 < p->tokens->size(p->tokens)) ? p->tokens->at(p->tokens, p->cursor + 1) : NULL;

        if (t1 && t1->kind == CPLUS_TOKEN_KIND_EQ) {
            advance(p);
            advance(p);
            AST *e = parse_expression(p);
            expect(p, CPLUS_TOKEN_KIND_SEMICOLON, "expected ';' after assignment");
            AST *let = node_new(AST_LET, name->line, name->column);
            let->let.name = sv_from(p, name);
            let->let.value = e;
            return let;
        }
    }

    AST *e = parse_expression(p);
    expect(p, CPLUS_TOKEN_KIND_SEMICOLON, "expected ';' after expression");

    AST *st = node_new(AST_EXPRSTMT, e ? e->line : 0, e ? e->column : 0);
    st->exprstmt.expr = e;
    return st;
}

static AST *parse_function(Parser *p)
{
    const CPlusToken *kw = peek(p);

    if (!kw || kw->kind != CPLUS_TOKEN_KIND_NAME) {
        return NULL;
    }

    StrView sv = sv_from(p, kw);

    if (!(sv.len == 3 && strncmp(sv.ptr, "def", 3) == 0)) {
        return NULL;
    }
    advance(p);

    const CPlusToken *name = peek(p);

    if (!name || name->kind != CPLUS_TOKEN_KIND_NAME) {
        p->error = (typeof(p->error)) {.panic = true, .msg = "expected function name", .line = kw->line, .column = kw->column};
        return NULL;
    }
    advance(p);
    expect(p, CPLUS_TOKEN_KIND_OPEN_PAREN, "expected '(' after function name");

    ASTList params = {0};

    if (!match(p, CPLUS_TOKEN_KIND_CLOSE_PAREN)) {
        parse_param_list(p, &params);
        expect(p, CPLUS_TOKEN_KIND_CLOSE_PAREN, "expected ')' after parameters");
    }

    expect(p, CPLUS_TOKEN_KIND_ARROW, "expected '->' before return type");

    AST *ret_ty = parse_type(p);
    AST *body = parse_block(p);
    AST *fn = node_new(AST_FUNC, kw->line, kw->column);

    fn->func.name = sv_from(p, name);
    fn->func.params = params;
    fn->func.ret_type = ret_ty;
    fn->func.body = body;
    return fn;
}

__cplus__used AST *parse_module(Parser *p)
{
    AST *m = node_new(AST_MODULE, 1, 1);

    for (;;) {
        const CPlusToken *t = peek(p);

        if (!t) {
            break;
        }

        AST *f = parse_function(p);

        if (!f) {
            p->error = (typeof(p->error)) {.panic = true, .msg = "expected top-level 'def'", .line = t->line, .column = t->column};
            break;
        }

        ast_list_push(&m->module.items, f);

        if (p->error.panic) {
            break;
        }
    }
    return m;
}
