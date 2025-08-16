#include <cplus/header.h>
#include <cplus/inst_types.h>
#include <cplus/io.h>

#include <fcntl.h>
#include <unistd.h>

// clang-format off
static constexpr CPlusInst program[4] = {
    {
        .type = INST_MOV_IMM,
        .destination = 0,
        .registr = {0},
        .immediate = 2
    },
    {
        .type = INST_MOV_IMM,
        .destination = 1,
        .registr = {0},
        .immediate = 3
    },
    {
        .type = INST_ADD,
        .destination = 2,
        .registr = {0, 1},
        .immediate = 0
    },
    {
        .type = INST_HALT,
        .destination = 0,
        .registr = {0, 0},
        .immediate = 0
    },
};

static constexpr  CPlusHeader header = {
    .magic = CPLUS_MAGIC_U8,
    .version = {CPLUS_VERSION_0, CPLUS_VERSION_1},
    .arch = CPLUS_ARCH_X86_64,
    .program_size = sizeof(program),
    .entry = 3,
};
// clang-format on

int main(void)
{
    IOStream stream;

    stream.filename = "example/dummy.cpo";
    stream.fd = open(stream.filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    stream.size = sizeof(header) + sizeof(program);

    write(stream.fd, &header, sizeof(header));
    write(stream.fd, program, sizeof(program));
    close(stream.fd);
    return 0;
}
