#include "cplus/lexer.h"
#include <cplus/header.h>
#include <error/assert.h>
#include <parse_arguments.h>

#include <stdio.h>

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

int main(const int argc, const char **argv)
{
    const Arguments args = parse_arguments(argc, argv);

    if (args.flags & ARGUMENT_FLAG_HELP) {
        return usage(argv[0]);
    }
    if (args.flags & ARGUMENT_FLAG_VERSION) {
        return version(argv[0]);
    }

    Lexer *lexer = new (LexerClass, args.inputs, args.output, args.flags);

    lexer->lex(lexer);
    lexer->show(lexer);

    return 0;
}
