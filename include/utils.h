#ifndef CPLUS_UTILS_H_
#define CPLUS_UTILS_H_

#include <cplus/types.h>
#include <memory/allocate.h>

#include <stdio.h>
#include <string.h>

static __inline const char *add_extension(const char *filename, const char *ext)
{
    u64 len = strlen(filename);
    const u64 ext_len = strlen(ext);
    char *new_filename;
    char *dot_pos;

    dot_pos = strrchr(filename, '.');
    if (dot_pos != NULL && dot_pos != filename) {
        len = (u64) dot_pos - (u64) filename;
    }
    allocate(new_filename, len + ext_len + 1);
    snprintf(new_filename, len + ext_len + 1, "%.*s%s", (int) len, filename, ext);
    return new_filename;
}

#endif /* CPLUS_UTILS_H_ */
