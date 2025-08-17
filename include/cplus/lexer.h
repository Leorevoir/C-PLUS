#ifndef CPLUS_LEXER_H_
#define CPLUS_LEXER_H_

#include <oop/array.h>

#include <cplus/io.h>
#include <cplus/types.h>

typedef enum {
    CPLUS_TOKEN_KIND_NAME,
    CPLUS_TOKEN_KIND_NUMBER,
    CPLUS_TOKEN_KIND_STRING,

    CPLUS_TOKEN_KIND_OPEN_PAREN,
    CPLUS_TOKEN_KIND_CLOSE_PAREN,
    CPLUS_TOKEN_KIND_OPEN_CURLY,
    CPLUS_TOKEN_KIND_CLOSE_CURLY,

    CPLUS_TOKEN_KIND_SEMICOLON,
    CPLUS_TOKEN_KIND_COLON,
    CPLUS_TOKEN_KIND_COMMA,
    CPLUS_TOKEN_KIND_DOT,
    CPLUS_TOKEN_KIND_QUOTE,
    CPLUS_TOKEN_KIND_ARROW,

    CPLUS_TOKEN_KIND_PLUS,
    CPLUS_TOKEN_KIND_MINUS,
    CPLUS_TOKEN_KIND_ASTERISK,
    CPLUS_TOKEN_KIND_SLASH,
    CPLUS_TOKEN_KIND_PERCENT,
    CPLUS_TOKEN_KIND_MODULO,

    CPLUS_TOKEN_KIND_EQ,
    CPLUS_TOKEN_KIND_NE,
    CPLUS_TOKEN_KIND_LT,
    CPLUS_TOKEN_KIND_GT,
    CPLUS_TOKEN_KIND_LE,
    CPLUS_TOKEN_KIND_GE,

    CPLUS_TOKEN_KIND_AND,
    CPLUS_TOKEN_KIND_OR,
    CPLUS_TOKEN_KIND_NOT,
    CPLUS_TOKEN_KIND_XOR,
    CPLUS_TOKEN_KIND_UNKNOWN,
} CPlusTokenKind;

__cplus__const static __inline const char *token_kind_to_string(const CPlusTokenKind kind)
{
    switch (kind) {
        case CPLUS_TOKEN_KIND_NAME:
            return "NAME";
        case CPLUS_TOKEN_KIND_NUMBER:
            return "NUMBER";
        case CPLUS_TOKEN_KIND_STRING:
            return "STRING";
        case CPLUS_TOKEN_KIND_OPEN_PAREN:
            return "OPEN_PAREN";
        case CPLUS_TOKEN_KIND_CLOSE_PAREN:
            return "CLOSE_PAREN";
        case CPLUS_TOKEN_KIND_OPEN_CURLY:
            return "OPEN_CURLY";
        case CPLUS_TOKEN_KIND_CLOSE_CURLY:
            return "CLOSE_CURLY";
        case CPLUS_TOKEN_KIND_SEMICOLON:
            return "SEMICOLON";
        case CPLUS_TOKEN_KIND_COLON:
            return "COLON";
        case CPLUS_TOKEN_KIND_COMMA:
            return "COMMA";
        case CPLUS_TOKEN_KIND_DOT:
            return "DOT";
        case CPLUS_TOKEN_KIND_QUOTE:
            return "QUOTE";
        case CPLUS_TOKEN_KIND_ARROW:
            return "ARROW";
        case CPLUS_TOKEN_KIND_PLUS:
            return "PLUS";
        case CPLUS_TOKEN_KIND_MINUS:
            return "MINUS";
        case CPLUS_TOKEN_KIND_ASTERISK:
            return "ASTERISK";
        case CPLUS_TOKEN_KIND_SLASH:
            return "SLASH";
        case CPLUS_TOKEN_KIND_PERCENT:
            return "PERCENT";
        case CPLUS_TOKEN_KIND_MODULO:
            return "MODULO";
        case CPLUS_TOKEN_KIND_EQ:
            return "EQ";
        case CPLUS_TOKEN_KIND_NE:
            return "NE";
        case CPLUS_TOKEN_KIND_LT:
            return "LT";
        case CPLUS_TOKEN_KIND_GT:
            return "GT";
        case CPLUS_TOKEN_KIND_LE:
            return "LE";
        case CPLUS_TOKEN_KIND_GE:
            return "GE";
        case CPLUS_TOKEN_KIND_AND:
            return "AND";
        case CPLUS_TOKEN_KIND_OR:
            return "OR";
        case CPLUS_TOKEN_KIND_NOT:
            return "NOT";
        case CPLUS_TOKEN_KIND_XOR:
            return "XOR";
        default:
            break;
    }
    return NULL;
}

typedef struct {
    CPlusTokenKind kind;
    u64 start;
    u64 end;
    u64 line;
    u64 column;
} CPlusToken;

typedef struct {
    IOStream stream;
    Array *tokens;
} CPlusFile;

typedef struct Lexer {
    const Class *class;

    void (*lex)(struct Lexer *self);
    void (*show)(const struct Lexer *self);

    CPlusFile input;
    int flags;
} Lexer;

__cplus__const const Class *LexerGetClass(void);
#define LexerClass LexerGetClass()

#endif /* CPLUS_LEXER_H_ */
