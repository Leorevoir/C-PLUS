#include <memory/allocate.h>
#include <oop/array.h>

#include "lexer_utils.h"

__cplus__used static void lexer_ctor(void *instance, va_list *args);
__cplus__used static void lexer_dtor(void *instance);

__cplus__used void lexer_show(const Lexer *self);

// clang-format off
static const Class __cplus__used lexer_class = {
    .__size__ = sizeof(Lexer),
    .__name__ = "Lexer",
    .__ctor__ = lexer_ctor,
    .__dtor__ = lexer_dtor,
};
// clang-format on

static void lexer_lex(Lexer *self)
{
    const CPlusFile *file = &self->input;
    u64 j = 0;
    u64 line = 1;
    u64 column = 1;

    LexerContext ctx = {.file = file, .pos = &j, .line = &line, .column = &column};

    while (j < file->io.size) {
        if (lexer_try_newline(&ctx) || lexer_try_whitespace(&ctx) || lexer_try_fixed_symbols(&ctx) || lexer_try_identifier(&ctx)
            || lexer_try_number(&ctx) || lexer_try_string(&ctx)) {
            continue;
        }

        const char c = lexer_current_char(&ctx);
        __assert(false, "unexpected character '%c' at position %lu in file '%s'", c, j, file->io.filename);
    }
}

__cplus__const const Class *LexerGetClass(void)
{
    return &lexer_class;
}

/**
* private
*/

static void lexer_ctor(void *instance, va_list *args)
{
    Lexer *self = (Lexer *) instance;

    self->class = LexerGetClass();
    self->lex = lexer_lex;
    self->show = lexer_show;

    self->input.io.filename = va_arg(*args, const char *);
    self->input.tokens = new (ArrayClass, sizeof(CPlusToken), 32);

    io_stream_allocate(&self->input.io, "rb");
    io_stream_read(&self->input.io);
}

static void lexer_dtor(void *instance)
{
    Lexer *self = (Lexer *) instance;

    io_stream_liberate(&self->input.io);
}
