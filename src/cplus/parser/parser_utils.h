#ifndef CPLUS_PARSER_UTILS_H_
#define CPLUS_PARSER_UTILS_H_

#include <memory/allocate.h>

#include <cplus/lexer.h>
#include <cplus/parser.h>

#include <string.h>

#define LIKELY(x) (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect(!!(x), 0))

__cplus__nodiscard static __inline const CPlusToken *peek(const Parser *p)
{
    if (p->cursor < p->tokens->size(p->tokens)) {
        return p->tokens->at(p->tokens, p->cursor);
    }
    return NULL;
}

static __inline const CPlusToken *advance(Parser *p)
{
    const CPlusToken *t = peek(p);

    if (t) {
        ++p->cursor;
    }
    return t;
}

static __inline bool match(Parser *p, CPlusTokenKind k)
{
    const CPlusToken *t = peek(p);

    if (t && t->kind == k) {
        ++p->cursor;
        return true;
    }

    return false;
}

static __inline void expect(Parser *p, CPlusTokenKind k, const char *msg)
{
    if (LIKELY(match(p, k))) {
        return;
    }

    const CPlusToken *t = peek(p);

    p->error = (typeof(p->error)) {
        .panic = true,
        .msg = msg,
        .line = t ? t->line : 0,
        .column = t ? t->column : 0,
    };
}

static __inline StrView sv_from(Parser *p, const CPlusToken *t)
{
    return (StrView) {.ptr = p->buffer + t->start, .len = t->end - t->start};
}

__cplus__nodiscard static __inline AST *node_new(ASTKind k, u64 line, u64 col)
{
    AST *n;

    allocate(n, sizeof(*n));
    memset(n, 0, sizeof(*n));
    n->kind = k;
    n->line = line;
    n->column = col;
    return n;
}

#endif /* CPLUS_PARSER_UTILS_H_ */
