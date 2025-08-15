#include <stdio.h>
#include <sys/stat.h>

static void load_from_file(const char *filename)
{
    printf("filename: %s\n", filename);
}

typedef struct {
    FILE *stream;
    struct stat st;
    char *buffer;
    const char *filename;
} IO_Stream;

typedef struct {
    IO_Stream io;
} VM;

int main(const int argc, const char **argv)
{
    return 0;
}
