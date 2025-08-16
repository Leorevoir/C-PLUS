#include "ld/linker.h"
#include <memory/allocate.h>

int main(const int argc, const char **argv)
{
    if (argc != 2) {
        return 84;
    }

    Linker *linker = new (LinkerClass, argv[1]);

    linker->link(linker);
    return 0;
}
