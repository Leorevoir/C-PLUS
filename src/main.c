#include <memory/allocate.h>
#include <vm/vm.h>

int main(const int argc, const char **argv)
{
    if (argc != 2) {
        return 84;
    }

    VM *vm = new (VMClass, argv[1]);

    vm->start(vm);
    return 0;
}
