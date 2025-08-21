#include <cplus/lexer.h>

__cplus__used void lexer_show(const Lexer *self);

void lexer_show(const Lexer *self)
{
    array_foreach(self->input.tokens, CPlusToken, tok, {
        printf("%sToken%s %-12s [", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET, token_kind_to_string(tok->kind));
        for (u64 j = tok->start; j < tok->end; j++) {
            putchar(self->input.io.buffer[j]);
        }
        printf("]      (pos %lu-%lu, line %lu, col %lu)\n", tok->start, tok->end, tok->line, tok->column);
    });
}
