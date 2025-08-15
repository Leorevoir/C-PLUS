#include "include/vm/header.h"
#include "include/vm/types.h"

// clang-format off
static constexpr Inst program[] = {
    {.type = INST_PUSH, .value = 10},
    { .type = INST_PUSH, .value = 20 },
    { .type = INST_ADD, .value = 0 },
    { .type = INST_HALT, .value = 0 }
};

static constexpr CPlusHeader header = {
    .magic = CPLUS_VM_MAGIC_HEADER_STR,
    .version = CPLUS_VM_STANDARD_VERSION,
    .file_size = sizeof(CPlusHeader) + sizeof(program),
    .instruction_count = sizeof(program) / sizeof(Inst)
};
// clang-format on

#include <stdio.h>

static void write_dummy_program(const char *filename)
{
    FILE *stream = fopen(filename, "wb");

    if (!stream) {
        return;
    }
    fwrite(&header, sizeof(header), 1, stream);
    fwrite(program, sizeof(program), 1, stream);
    fclose(stream);
}

int main(void)
{
    write_dummy_program("example/program.cpo");
}
