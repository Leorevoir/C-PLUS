#include <string.h>
#include <vm/header.h>
#include <vm/vm.h>

__cplus__used static void vm_ctor(void *instance, va_list *args);
__cplus__used static void vm_dtor(void *instance);

static __inline void ensure_valid_header(const uint32_t *buff)
{
    __assert(buff, "buffer is NULL");

    const CPlusHeader *header = (const CPlusHeader *) buff;

    __assert(memcmp(header->magic, CPLUS_VM_MAGIC_HEADER_STR, CPLUS_VM_MAGIC_NUMBER_SIZE) == 0, "invalid magic value");
}

// clang-format off
static const Class __cplus__used vm_class = {
    .__size__ = sizeof(VM),
    .__name__ = "VM",
    .__ctor__ = vm_ctor,
    .__dtor__ = vm_dtor,
};
// clang-format on

/**
* public
*/

__cplus__const __cplus__used const Class *VMGetClass(void)
{
    return &vm_class;
}

static void vm_start(VM *self)
{
    struct _VMData *priv = &self->_priv;

    io_file_exists(&priv->_io);
    io_file_read(&priv->_io);
    ensure_valid_header(priv->_io.buff);
}

/**
* private
*/

static void vm_ctor(Object *instance, va_list *args)
{
    VM *self = (VM *) instance;
    struct _VMData *priv = &self->_priv;

    self->class = VMGetClass();
    self->start = vm_start;

    priv->_io.src = va_arg(*args, const char *);
}

static void vm_dtor(Object __cplus__unused *instance)
{
    /* __nothing__ */
}
