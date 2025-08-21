#include "cpa/cpa.h"
#include "cplus/lexer.h"
#include "cplus/parser.h"
#include "memory/garbage_collector.h"
#include <cplus/header.h>
#include <error/assert.h>
#include <parse_arguments.h>

#include <stdio.h>

static Arguments args;

static __inline int usage(const char *program)
{
    printf("%sUSAGE:%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
    printf("\t%s [options] <input files>\n", program);
    printf("%sOPTIONS:%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
    printf("\t%s-h%s\t\tDisplay this help message\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET);
    printf("\t%s-v%s\t\tDisplay the version of the compiler\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET);
    printf("\t%s-o <file>%s\tSpecify the output file\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET);
    printf("%sARGUMENTS:%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
    printf("\t%s<input files>%s\tList of input files to compile\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET);
    return 0;
}

static __inline int version(const char *program)
{
    printf("%sVERSION:%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
    printf("\t%s v%d.%d\n", program, CPLUS_VERSION_MAJOR, CPLUS_VERSION_MINOR);
    return 0;
}

static __inline Lexer *cplus_compiler_lexer(const char *input)
{
    Lexer *lexer = new (LexerClass, input);

    lexer->lex(lexer);
    if (args.flags & ARGUMENT_FLAG_DEBUG) {
        lexer->show(lexer);
    }
    return lexer;
}

static __inline Parser *cplus_compiler_parser(const Lexer *lexer)
{
    Parser *parser = new (ParserClass, lexer);

    parser->ast(parser);
    if (args.flags & ARGUMENT_FLAG_DEBUG) {
        parser->show(parser);
    }
    return parser;
}

static __inline void cplus_compiler_routine(void)
{
    array_foreach(args.inputs, const char *, input, {
        Lexer *lexer = cplus_compiler_lexer(*input);
        Parser *parser = cplus_compiler_parser(lexer);
        CPAssembly *assembly = new (CPAssemblyClass, parser->root, *input);

        assembly->assemble(assembly);
        delete (lexer);
        delete (parser);
        delete (assembly);
    });
}

int main(const int argc, const char **argv)
{
    args = parse_arguments(argc, argv);

    if (args.flags & ARGUMENT_FLAG_HELP) {
        return usage(argv[0]);
    }
    if (args.flags & ARGUMENT_FLAG_VERSION) {
        return version(argv[0]);
    }

    cplus_compiler_routine();
    return 0;
}
