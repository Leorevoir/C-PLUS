#include <memory/allocate.h>
#include <vm/vm.h>

int main(void)
{
    VM *vm = new (VMClass, "example/example.cp.asm");

    vm->start(vm);
    return 0;
}
