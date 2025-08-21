#include <memory/allocate.h>
#include <parse_arguments.h>

static __inline void parse_output_flag(const int argc, const char **argv, int *out_i, Arguments *out_args)
{
    __assert(*out_i + 1 < argc, "no output file specified after -o flag");
    const char *next = argv[*out_i + 1];
    __assert(next[0] != '-' && (next[1] != 'o' || next[1] != 'h' || next[1] != 'v' || next[1] != 'd'),
        "expected output filename after -o, got a flag instead");

    out_args->output = next;
    ++(*out_i);
}

static void parse_arguments_flags(const int argc, const char **argv, int *out_i, Arguments *out_args)
{
    switch (argv[*out_i][1]) {

        case 'o':
            parse_output_flag(argc, argv, out_i, out_args);
            break;

        case 'h':
            out_args->flags |= ARGUMENT_FLAG_HELP;
            break;

        case 'v':
            out_args->flags |= ARGUMENT_FLAG_VERSION;
            break;

        case 'd':
            out_args->flags |= ARGUMENT_FLAG_DEBUG;
            break;

        default:
            __assert(false, "unknown argument flag! plz read the documentation or -h idk.........");
            break;
    }
}

Arguments parse_arguments(const int argc, const char **argv)
{
    __assert(argc > 1, "no arguments specified");
    Arguments args = {.flags = ARGUMENT_FLAG_NONE, .inputs = new (ArrayClass, sizeof(const char *), 1), .output = "zebi"};

    for (int i = 1; i < argc; ++i) {

        if (argv[i][0] == '-') {
            parse_arguments_flags(argc, argv, &i, &args);
            continue;
        }

        args.inputs->append(args.inputs, &argv[i]);
    }

    if (args.flags & ARGUMENT_FLAG_NONE) {
        __assert(args.inputs->size(args.inputs) > 0, "no input files specified");
    }
    return args;
}
