#ifndef CPLUS_AST_H_
#define CPLUS_AST_H_

#include <cplus/types.h>
#include <memory/allocate.h>

typedef enum {
    AST_MODULE,
    AST_FUNC,
    AST_PARAM,
    AST_TYPE,
    AST_BLOCK,
    AST_RETURN,
    AST_LET,
    AST_CALL,
    AST_BINOP,
    AST_UNOP,
    AST_IDENT,
    AST_NUMBER,
    AST_STRING,
    AST_EXPRSTMT
} ASTKind;

typedef struct {
    const char *ptr;
    u64 len;
} StrView;
typedef struct AST AST;

typedef struct {
    AST **data;
    u64 len;
    u64 cap;
} ASTList;

typedef struct {
    ASTList items;
} ASTModule;

typedef struct {
    StrView name;
    ASTList params;
    AST *ret_type;
    AST *body;
} ASTFunc;

typedef struct {
    StrView name;
    AST *type;
} ASTParam;

typedef struct {
    StrView name;
} ASTType;

typedef struct {
    ASTList items;
} ASTBlock;

typedef struct {
    AST *value;
} ASTReturn;

typedef struct {
    StrView name;
    AST *value;
} ASTLet;

typedef struct {
    StrView name;
    ASTList args;
} ASTCall;

typedef struct {
    AST *left;
    AST *right;
    StrView op;
} ASTBinOp;

typedef struct {
    StrView op;
    AST *operand;
} ASTUnOp;

typedef struct {
    StrView name;
} ASTIdent;

typedef struct {
    StrView value;
} ASTNumber;

typedef struct {
    StrView value;
} ASTString;

typedef struct {
    AST *expr;
} ASTExprStmt;

struct AST {
    ASTKind kind;
    u64 line;
    u64 column;

    union {
        ASTModule module;
        ASTFunc func;
        ASTParam param;
        ASTType type;
        ASTBlock block;
        ASTReturn ret;
        ASTLet let;
        ASTCall call;
        ASTBinOp binop;
        ASTUnOp unop;
        ASTIdent ident;
        ASTNumber number;
        ASTString string;
        ASTExprStmt exprstmt;
    };
};

static __inline void ast_list_init(ASTList *list)
{
    list->data = NULL;
    list->len = 0;
    list->cap = 0;
}

static __inline void ast_list_push(ASTList *list, AST *new)
{
    if (list->len == list->cap) {
        const size_t n = vectorize_size(list->cap);

        reallocate(list->data, n * sizeof *list->data);
        list->cap = n;
    }
    list->data[list->len++] = new;
}

#endif /* CPLUS_AST_H_ */
