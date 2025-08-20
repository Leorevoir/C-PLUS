#ifndef CPLUS_PARSER_H_
#define CPLUS_PARSER_H_

#include <oop/array.h>
#include <oop/interface.h>
#include <oop/macros.h>

#include <cplus/ast.h>

typedef struct Parser {
    const Class *class;

    void (*ast)(struct Parser *self);
    void (*show)(const struct Parser *self);

    const Array *tokens;
    const char *buffer;
    const char *input;
    u64 buffer_size;
    u64 cursor;

    struct {
        bool panic;
        const char *msg;
        u64 line;
        u64 column;
    } error;

    AST *root;
} Parser;

__cplus__const const Class *ParserGetClass(void);
#define ParserClass ParserGetClass()

#endif /* CPLUS_PARSER_H_ */
