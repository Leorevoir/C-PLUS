#include <cplus/lexer.h>
#include <cplus/parser.h>

#include <oop/array.h>
#include <parse_arguments.h>
#include <string.h>

void parser_show(const Parser *self);
AST *parse_module(Parser *self);

__cplus__used static void parser_ctor(void *instance, va_list *args);
__cplus__used static void parser_dtor(void *instance);

// clang-format off
static const Class __cplus__used parser_class = {
    .__size__ = sizeof(Parser),
    .__name__ = "Parser",
    .__ctor__ = parser_ctor,
    .__dtor__ = parser_dtor,
};
// clang-format on

/**
* public
*/

__cplus__const const Class *ParserGetClass(void)
{
    return &parser_class;
}

static void parser_ast(Parser *self)
{
    self->cursor = 0;
    self->error = (typeof(self->error)) {0};
    self->root = parse_module(self);
}

/*
* private
*/

static __inline const char *_parser_get_only_filename(const char *absolute_input)
{
    const char *filename = strrchr(absolute_input, '/');

    if (filename != NULL) {
        return ++filename;
    }
    return absolute_input;
}

static void parser_ctor(void *instance, va_list *args)
{
    Parser *self = (Parser *) instance;

    self->class = ParserGetClass();
    self->ast = parser_ast;
    self->show = parser_show;

    const Lexer *lexer = va_arg(*args, const Lexer *);

    self->input = _parser_get_only_filename(lexer->input.io.filename);
    self->tokens = lexer->input.tokens;
    self->buffer = (const char *) lexer->input.io.buffer;
    self->buffer_size = lexer->input.io.size;
    self->root = NULL;
}

static void parser_dtor(void __cplus__unused *instance)
{
    /* __garbaged__collected__ */
}
