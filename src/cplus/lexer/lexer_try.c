#include "lexer_utils.h"

#include <ctype.h>
#include <string.h>

struct TokenMap {
    const CPlusTokenKind kind;
    const char *repr;
};

// clang-format off
static const struct TokenMap token_map[] = {
    { CPLUS_TOKEN_KIND_ARROW,       "->" },
    { CPLUS_TOKEN_KIND_NE,          "!=" },
    { CPLUS_TOKEN_KIND_LE,          "<=" },
    { CPLUS_TOKEN_KIND_GE,          ">=" },
    { CPLUS_TOKEN_KIND_AND,         "&&" },
    { CPLUS_TOKEN_KIND_OR,          "||" },
    { CPLUS_TOKEN_KIND_OPEN_PAREN,  "("  },
    { CPLUS_TOKEN_KIND_CLOSE_PAREN, ")"  },
    { CPLUS_TOKEN_KIND_OPEN_CURLY,  "{"  },
    { CPLUS_TOKEN_KIND_CLOSE_CURLY, "}"  },
    { CPLUS_TOKEN_KIND_SEMICOLON,   ";"  },
    { CPLUS_TOKEN_KIND_COLON,       ":"  },
    { CPLUS_TOKEN_KIND_COMMA,       ","  },
    { CPLUS_TOKEN_KIND_DOT,         "."  },
    // { CPLUS_TOKEN_KIND_QUOTE,       "\""  }, INFO: commented bc handled by string
    { CPLUS_TOKEN_KIND_PLUS,        "+"  },
    { CPLUS_TOKEN_KIND_MINUS,       "-"  },
    { CPLUS_TOKEN_KIND_ASTERISK,    "*"  },
    { CPLUS_TOKEN_KIND_SLASH,       "/"  },
    { CPLUS_TOKEN_KIND_MODULO,      "%" },
    { CPLUS_TOKEN_KIND_EQ,          "="  },
    { CPLUS_TOKEN_KIND_LT,          "<"  },
    { CPLUS_TOKEN_KIND_GT,          ">"  },
    { CPLUS_TOKEN_KIND_NOT,         "!"  },
    { CPLUS_TOKEN_KIND_XOR,         "^"  },
    { CPLUS_TOKEN_KIND_UNKNOWN,     NULL },
};
// clang-format on

static __inline void lexer_emit_token(const LexerContext *ctx, CPlusTokenKind kind, u64 start, u64 end, u64 start_col)
{
    const CPlusToken token = {.kind = kind, .start = start, .end = end, .line = *ctx->line, .column = start_col};
    ctx->file->tokens->append(ctx->file->tokens, &token);
}

bool lexer_try_newline(LexerContext *ctx)
{
    if (lexer_current_char(ctx) == '\n') {
        lexer_advance_newline(ctx);
        return true;
    }
    return false;
}

bool lexer_try_fixed_symbols(LexerContext *ctx)
{
    const char *p = (const char *) ctx->file->io.buffer + *ctx->pos;

    for (u64 k = 0; token_map[k].repr; ++k) {
        const u64 len = strlen(token_map[k].repr);

        if (lexer_has_chars(ctx, len) && strncmp(p, token_map[k].repr, len) == 0) {
            lexer_emit_token(ctx, token_map[k].kind, *ctx->pos, *ctx->pos + len, *ctx->column);
            lexer_advance(ctx, len);
            return true;
        }
    }
    return false;
}

bool lexer_try_identifier(LexerContext *ctx)
{
    const char c = lexer_current_char(ctx);

    if (!isalpha((unsigned char) c) && c != '_') {
        return false;
    }

    const u64 start = *ctx->pos;
    const u64 start_col = *ctx->column;

    lexer_advance(ctx, 1);

    while (*ctx->pos < ctx->file->io.size) {
        const char ch = lexer_current_char(ctx);
        if (!isalnum((unsigned char) ch) && ch != '_') {
            break;
        }
        lexer_advance(ctx, 1);
    }

    lexer_emit_token(ctx, CPLUS_TOKEN_KIND_NAME, start, *ctx->pos, start_col);
    return true;
}

bool lexer_try_number(LexerContext *ctx)
{
    const char c = lexer_current_char(ctx);

    if (!isdigit((unsigned char) c)) {
        return false;
    }

    const u64 start = *ctx->pos;
    const u64 start_col = *ctx->column;

    lexer_advance(ctx, 1);

    while (*ctx->pos < ctx->file->io.size) {
        const char ch = lexer_current_char(ctx);
        if (!isdigit((unsigned char) ch) && ch != '.') {
            break;
        }
        lexer_advance(ctx, 1);
    }

    lexer_emit_token(ctx, CPLUS_TOKEN_KIND_NUMBER, start, *ctx->pos, start_col);
    return true;
}

bool lexer_try_string(LexerContext *ctx)
{
    const char c = lexer_current_char(ctx);

    if (c != '"' && c != '\'') {
        return false;
    }

    const char quote = c;
    const u64 start = *ctx->pos;
    const u64 start_col = *ctx->column;

    lexer_advance(ctx, 1);

    while (*ctx->pos < ctx->file->io.size && lexer_current_char(ctx) != quote) {
        const char ch = lexer_current_char(ctx);

        if (ch == '\\' && lexer_has_chars(ctx, 2)) {
            lexer_advance(ctx, 2);
        } else if (ch == '\n') {
            lexer_advance_newline(ctx);
        } else {
            lexer_advance(ctx, 1);
        }
    }

    if (*ctx->pos < ctx->file->io.size && lexer_current_char(ctx) == quote) {
        lexer_advance(ctx, 1);
    }

    lexer_emit_token(ctx, CPLUS_TOKEN_KIND_STRING, start, *ctx->pos, start_col);
    return true;
}

bool lexer_try_whitespace(LexerContext *ctx)
{
    const char c = lexer_current_char(ctx);

    if (!isspace((unsigned char) c) || c == '\n') {
        return false;
    }

    if (c == '\t') {
        // TODO: find better way for tabs bc we assume tab is 4 spaces
        *ctx->pos += 1;
        *ctx->column += 4;
    } else {
        lexer_advance(ctx, 1);
    }

    return true;
}
