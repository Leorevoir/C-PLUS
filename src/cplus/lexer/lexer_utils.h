#ifndef CPLUS_LEXER_UTILS_H_
#define CPLUS_LEXER_UTILS_H_

#include <cplus/lexer.h>

typedef struct {
    const CPlusFile *file;
    u64 *pos;
    u64 *line;
    u64 *column;
} LexerContext;

static __inline char lexer_current_char(const LexerContext *ctx)
{
    return (*ctx->pos < ctx->file->io.size) ? (char) ctx->file->io.buffer[*ctx->pos] : '\0';
}

static __inline bool lexer_has_chars(const LexerContext *ctx, u64 count)
{
    return *ctx->pos + count <= ctx->file->io.size;
}

static __inline void lexer_advance(const LexerContext *ctx, u64 count)
{
    *ctx->pos += count;
    *ctx->column += count;
}

static __inline void lexer_advance_newline(const LexerContext *ctx)
{
    (*ctx->pos)++;
    (*ctx->line)++;
    *ctx->column = 1;
}

bool lexer_try_newline(LexerContext *ctx);
bool lexer_try_fixed_symbols(LexerContext *ctx);
bool lexer_try_identifier(LexerContext *ctx);
bool lexer_try_number(LexerContext *ctx);
bool lexer_try_string(LexerContext *ctx);
bool lexer_try_whitespace(LexerContext *ctx);

#endif /* CPLUS_LEXER_UTILS_H_ */
