#ifndef CPLUS_LD_IO_H_
#define CPLUS_LD_IO_H_

#include <ld/types.h>

#include <sys/stat.h>

typedef struct {
    int fd;
    const char *filename;
    u8 *buffer;
    u64 size;
    struct stat st;
} IOStream;

#endif /* CPLUS_LD_IO_H_ */
