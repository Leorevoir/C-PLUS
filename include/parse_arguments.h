#ifndef CPLUS_PARSE_ARGUMENTS_H_
#define CPLUS_PARSE_ARGUMENTS_H_

#include <oop/array.h>

typedef enum {
    ARGUMENT_FLAG_NONE = 0,
    ARGUMENT_FLAG_OUTPUT = 1 << 0,
    ARGUMENT_FLAG_HELP = 1 << 1,
    ARGUMENT_FLAG_VERSION = 1 << 2,
    ARGUMENT_FLAG_DEBUG = 1 << 3,
} ArgumentFlags;

typedef struct {
    int flags;
    Array *inputs;
    const char *output;
} Arguments;

Arguments parse_arguments(const int argc, const char **argv);

#endif /* CPLUS_PARSE_ARGUMENTS_H_ */
