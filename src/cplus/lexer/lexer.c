#include <cplus/lexer.h>
#include <memory/allocate.h>
#include <oop/array.h>

#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

__cplus__used static void lexer_ctor(void *instance, va_list *args);
__cplus__used static void lexer_dtor(void *instance);

// clang-format off
static const Class __cplus__used lexer_class = {
    .__size__ = sizeof(Lexer),
    .__name__ = "Lexer",
    .__ctor__ = lexer_ctor,
    .__dtor__ = lexer_dtor,
};

struct TokenMap {
  const CPlusTokenKind kind;
    const char *repr;
};

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
    { CPLUS_TOKEN_KIND_QUOTE,       "\""  },
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

/*
* public
*/

__cplus__const const Class *LexerGetClass(void)
{
    return &lexer_class;
}

static void lexer_show(const Lexer *self)
{
    array_foreach(self->input.tokens, CPlusToken, tok, {
        printf("%sToken%s %-12s [", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET, token_kind_to_string(tok->kind));
        for (u64 j = tok->start; j < tok->end; j++) {
            putchar(self->input.stream.buffer[j]);
        }
        printf("]      (pos %lu-%lu, line %lu, col %lu)\n", tok->start, tok->end, tok->line, tok->column);
    });
}

static void lexer_lex(Lexer *self)
{
    const CPlusFile *file = &self->input;
    u64 j = 0;
    u64 line = 1;
    u64 column = 1;

    while (j < file->stream.size) {
        const char *p = (const char *) file->stream.buffer + j;

        /** newlines for line/column tracking */
        if (*p == '\n') {
            ++line;
            column = 1;
            ++j;
            continue;
        }

        /** check fixed symbols/operators from token_map */
        bool matched = false;
        for (u64 k = 0; token_map[k].repr; ++k) {
            const u64 len = strlen(token_map[k].repr);

            if (j + len <= file->stream.size && strncmp(p, token_map[k].repr, len) == 0) {

                const CPlusToken token = {.kind = token_map[k].kind, .start = j, .end = j + len, .line = line, .column = column};
                file->tokens->append(file->tokens, &token);

                j += len;
                column += len;
                matched = true;
                break;
            }
        }
        if (matched) {
            continue;
        }

        /** identifiers (name / keywords) */
        if (isalpha((unsigned char) *p) || *p == '_') {
            const u64 start = j++;
            const u64 start_col = column++;

            while (j < file->stream.size && (isalnum((unsigned char) file->stream.buffer[j]) || file->stream.buffer[j] == '_')) {
                ++j;
                ++column;
            }
            const CPlusToken token = {.kind = CPLUS_TOKEN_KIND_NAME, .start = start, .end = j, .line = line, .column = start_col};
            file->tokens->append(file->tokens, &token);
            continue;
        }

        /** numbers (digits/dots idc for now) */
        if (isdigit((unsigned char) *p)) {
            const u64 start = j++;
            const u64 start_col = column++;
            while (j < file->stream.size && (isdigit((unsigned char) file->stream.buffer[j]) || file->stream.buffer[j] == '.')) {
                ++j;
                ++column;
            }
            const CPlusToken token = {.kind = CPLUS_TOKEN_KIND_NUMBER, .start = start, .end = j, .line = line, .column = start_col};
            file->tokens->append(file->tokens, &token);
            continue;
        }

        /** strings (double or single quoted) */
        if (*p == '"' || *p == '\'') {
            const char quote = *p;
            const u64 start = j++;
            const u64 start_col = column++;

            while (j < file->stream.size && file->stream.buffer[j] != quote) {
                if (file->stream.buffer[j] == '\\' && j + 1 < file->stream.size) {
                    j += 2;
                    column += 2;
                } else {
                    if (file->stream.buffer[j] == '\n') {
                        ++line;
                        column = 1;
                    } else {
                        ++column;
                    }
                    ++j;
                }
            }

            if (j < file->stream.size) {
                ++j;
                ++column;
            }

            const CPlusToken token = {.kind = CPLUS_TOKEN_KIND_STRING, .start = start, .end = j, .line = line, .column = start_col};
            file->tokens->append(file->tokens, &token);
            continue;
        }

        /** trim */
        if (isspace((unsigned char) *p)) {
            //TODO: find better way for tabs bc we assume tab is 4 spaces
            if (*p == '\t') {
                column += 4;
            } else {
                column++;
            }
            j++;
            continue;
        }

        __assert(false, "unexpected character '%c' at position %lu in file '%s'", *p, j, file->stream.filename);
    }
}

/**
* private
*/

static __inline void lexer_open_read_stream(IOStream *stream)
{
    stream->fd = open(stream->filename, O_RDONLY);
    __assert(stream->fd >= 0, "failed to open input file: %s", stream->filename);
    __assert(fstat(stream->fd, &stream->st) == 0, "failed to stat input file: %s", stream->filename);
    stream->size = (u64) stream->st.st_size;
    allocate(stream->buffer, stream->size + 1);
    const ssize_t br = read(stream->fd, stream->buffer, stream->size);
    __assert(br == (ssize_t) stream->size, "failed to read input file: %s", stream->filename);
    stream->buffer[stream->size] = '\0';
}

static __inline void lexer_load_inputs(CPlusFile *out_input)
{
    out_input->tokens = new (ArrayClass, sizeof(CPlusToken), 32);
    lexer_open_read_stream(&out_input->stream);
}

static void lexer_ctor(void *instance, va_list *args)
{
    Lexer *self = (Lexer *) instance;

    self->class = LexerGetClass();
    self->lex = lexer_lex;
    self->show = lexer_show;
    self->input.stream.filename = va_arg(*args, const char *);
    lexer_load_inputs(&self->input);
}

static void lexer_dtor(void *instance)
{
    Lexer *self = (Lexer *) instance;

    if (self->input.stream.fd >= 0) {
        close(self->input.stream.fd);
        self->input.stream.fd = -1;
    }
}
