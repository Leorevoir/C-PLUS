#include <stdio.h>
#include <sys/stat.h>

typedef struct {
    FILE *stream;
    struct stat st;
    char *buffer;
    const char *filename;
} IO_Stream;

typedef struct {
    IO_Stream io;
} VM;

int main(void)
{
    return 0;
}
