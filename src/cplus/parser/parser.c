#include <cplus/lexer.h>
#include <cplus/parser.h>

#include <oop/array.h>
#include <parse_arguments.h>

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

static void parser_ctor(void *instance, va_list *args)
{
    Parser *self = (Parser *) instance;

    self->class = ParserGetClass();
    self->ast = parser_ast;
    self->show = parser_show;

    Lexer *lexer = va_arg(*args, Lexer *);

    lexer->lex(lexer);

    if (lexer->flags & ARGUMENT_FLAG_DEBUG) {
        lexer->show(lexer);
    }

    self->tokens = lexer->input.tokens;
    self->buffer = (const char *) lexer->input.stream.buffer;
    self->buffer_size = lexer->input.stream.size;
    self->root = NULL;
}

static void parser_dtor(void *instance)
{
    (void) instance;
}
