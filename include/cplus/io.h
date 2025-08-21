#ifndef CPLUS_LD_IO_H_
#define CPLUS_LD_IO_H_

#include <cplus/types.h>
#include <error/assert.h>
#include <memory/allocate.h>
#include <memory/liberate.h>

#include <stdio.h>
#include <sys/stat.h>

typedef struct {
    FILE *stream;
    const char *filename;
    u8 *buffer;
    u64 size;
    struct stat st;
} IOStream;

static __inline void io_stream_read(IOStream *stream)
{
    __assert(stream->stream, "IOStream must be allocated before reading");
    __assert(stream->buffer, "buffer must be allocated before reading");
    __assert(stream->size > 0, "size must be set before reading");

    const size_t bytes_read = fread(stream->buffer, 1, stream->size, stream->stream);
    __assert(bytes_read == stream->size, "failed to read expected size from file: %s", stream->filename);
    stream->buffer[stream->size] = '\0';
}

static __inline void io_stream_allocate(IOStream *stream, const char *mode)
{
    __assert(stream->filename, "filename must be set before allocating IOStream");
    __assert(mode, "mode must be set before allocating IOStream");
    stream->stream = fopen(stream->filename, mode);
    __assert(stream->stream, "failed to open file: %s", stream->filename);
    if (mode[0] == 'r') {
        __assert(stat(stream->filename, &stream->st) == 0, "failed to stat file: %s", stream->filename);
        stream->size = (u64) stream->st.st_size;
        allocate(stream->buffer, stream->size + 1);
    }
}

static __inline void io_stream_liberate(IOStream *stream)
{
    if (stream->stream) {
        fclose(stream->stream);
    }
    liberate(stream->buffer);
    stream->stream = NULL;
    stream->buffer = NULL;
    stream->size = 0;
}

#endif /* CPLUS_LD_IO_H_ */
