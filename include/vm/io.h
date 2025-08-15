#ifndef C_PLUS_VM_IO_H_
#define C_PLUS_VM_IO_H_

#include <error/assert.h>
#include <memory/allocate.h>

#include <stdio.h>
#include <sys/stat.h>

typedef struct {
    FILE *stream;
    struct stat st;
    char *buff;
    const char *src;
} IO_Stream;

static __inline void io_file_exists(IO_Stream *out_io)
{
    __assert(stat(out_io->src, &out_io->st) == 0, "file does not exist");
    __assert(S_ISREG(out_io->st.st_mode) || S_ISLNK(out_io->st.st_mode), "file is not a regular file or symlink");
}

static __inline void io_file_read(IO_Stream *out_io)
{
    out_io->stream = fopen(out_io->src, "r");
    allocate(out_io->buff, (const size_t) out_io->st.st_size + 1);

    const size_t br = fread(out_io->buff, sizeof(char), (size_t) out_io->st.st_size, out_io->stream);

    __assert(br == (const size_t) out_io->st.st_size && !feof(out_io->stream), "failed to read file");
    fclose(out_io->stream);
    out_io->buff[out_io->st.st_size] = '\0';
}

#endif /* C_PLUS_VM_IO_H_ */
