#include <cplus/lexer.h>
#include <cplus/parser.h>

#include "parser_utils.h"

#include <string.h>

AST *parse_expression(Parser *p);
static AST *parse_primary(Parser *p);

/**
 * helpers
 */

static int precedence_of(CPlusTokenKind k)
{
    switch (k) {
        case CPLUS_TOKEN_KIND_ASTERISK:
        case CPLUS_TOKEN_KIND_SLASH:
        case CPLUS_TOKEN_KIND_MODULO:
            return 70;
        case CPLUS_TOKEN_KIND_PLUS:
        case CPLUS_TOKEN_KIND_MINUS:
            return 60;
        case CPLUS_TOKEN_KIND_LT:
        case CPLUS_TOKEN_KIND_GT:
        case CPLUS_TOKEN_KIND_LE:
        case CPLUS_TOKEN_KIND_GE:
            return 50;
        case CPLUS_TOKEN_KIND_EQ:
        case CPLUS_TOKEN_KIND_NE:
            return 45;
        case CPLUS_TOKEN_KIND_AND:
            return 40;
        case CPLUS_TOKEN_KIND_OR:
            return 30;
        default:
            return -1;
    }
}

static AST *parse_atom(Parser *p)
{
    const CPlusToken *t = peek(p);

    if (!t) {
        return NULL;
    }
    switch (t->kind) {
        case CPLUS_TOKEN_KIND_NUMBER: {
            advance(p);
            AST *n = node_new(AST_NUMBER, t->line, t->column);
            n->number.value = sv_from(p, t);
            return n;
        }
        case CPLUS_TOKEN_KIND_STRING: {
            advance(p);
            AST *n = node_new(AST_STRING, t->line, t->column);
            n->string.value = sv_from(p, t);
            return n;
        }
        case CPLUS_TOKEN_KIND_NAME: {
            advance(p);
            AST *n = node_new(AST_IDENT, t->line, t->column);
            n->ident.name = sv_from(p, t);
            return n;
        }
        case CPLUS_TOKEN_KIND_OPEN_PAREN: {
            advance(p);
            AST *expr = parse_expression(p);
            expect(p, CPLUS_TOKEN_KIND_CLOSE_PAREN, "expected ')'");
            return expr;
        }
        case CPLUS_TOKEN_KIND_NOT:
        case CPLUS_TOKEN_KIND_MINUS: {
            advance(p);
            AST *e = parse_atom(p);
            AST *n = node_new(AST_UNOP, t->line, t->column);
            n->unop.op = sv_from(p, t);
            n->unop.operand = e;
            return n;
        }
        default:
            return NULL;
    }
}

static AST *maybe_call(Parser *p, AST *left)
{
    for (;;) {
        const CPlusToken *t = peek(p);

        if (!t || t->kind != CPLUS_TOKEN_KIND_OPEN_PAREN) {
            break;
        }
        AST *call = node_new(AST_CALL, left->line, left->column);

        call->call.name = (left->kind == AST_IDENT) ? left->ident.name : (StrView) {.ptr = "<expr>", .len = 6};

        const CPlusToken *pk = peek(p);

        if (pk && pk->kind != CPLUS_TOKEN_KIND_CLOSE_PAREN) {
            for (;;) {
                AST *arg = parse_expression(p);
                ast_list_push(&call->call.args, arg);

                if (!match(p, CPLUS_TOKEN_KIND_COMMA)) {
                    break;
                }
            }
        }
        expect(p, CPLUS_TOKEN_KIND_CLOSE_PAREN, "expected ')'");
        left = call;
    }
    return left;
}

static AST *parse_primary(Parser *p)
{
    AST *a = parse_atom(p);

    if (!a) {
        return NULL;
    }
    return maybe_call(p, a);
}

static AST *parse_bp(Parser *p, const int min_bp)
{
    AST *left = parse_primary(p);

    if (!left) {
        return NULL;
    }

    for (;;) {
        const CPlusToken *t = peek(p);
        if (!t) {
            break;
        }

        const int prec = precedence_of(t->kind);
        if (prec < min_bp) {
            break;
        }

        StrView op = sv_from(p, t);
        advance(p);

        AST *right = parse_bp(p, prec + 1);
        AST *bin = node_new(AST_BINOP, t->line, t->column);

        bin->binop.op = op;
        bin->binop.left = left;
        bin->binop.right = right;
        left = bin;
    }

    return left;
}

/**
* entry point
*/

AST *parse_expression(Parser *p)
{
    return parse_bp(p, 0);
}
