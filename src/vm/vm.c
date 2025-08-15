#include <vm/header.h>
#include <vm/vm.h>

#include "vm_helper.h"

#include <string.h>

__cplus__used static void vm_ctor(void *instance, va_list *args);
__cplus__used static void vm_dtor(void *instance);

static __inline __cplus__const const CPlusHeader *get_valid_header(const uint32_t *buff)
{
    __assert(buff, "buffer is NULL");

    const CPlusHeader *header = (const CPlusHeader *) buff;

    __assert(memcmp(header->magic, CPLUS_VM_MAGIC_HEADER_STR, CPLUS_VM_MAGIC_NUMBER_SIZE) == 0, "invalid magic value");
    return header;
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

    priv->_header = get_valid_header(priv->_io.buff);
    priv->_program = (const Inst *) ((uint8_t *) priv->_io.buff + sizeof(CPlusHeader));
    priv->_program_size = (const size_t) priv->_io.st.st_size - sizeof(CPlusHeader);
    priv->_inst_count = priv->_program_size / sizeof(Inst);

    for (size_t i = 0; i < priv->_inst_count; ++i) {
        const Inst *inst = &priv->_program[i];

        dispatch_instruction(&self->stack, inst);
    }
}

static void vm_show(const VM *self)
{
    const struct _VMData *priv = &self->_priv;

    printf("program contains %zu instructions:\n", priv->_inst_count);
    for (size_t i = 0; i < priv->_inst_count; ++i) {
        const Inst *inst = &priv->_program[i];

        printf("%04zu: %-8s %d\n", i, inst_to_str(inst), inst->value);
    }
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
    self->show = vm_show;

    self->stack.max = CPLUS_VM_DEFAULT_MAX_STACK_SIZE;
    self->stack.size = 0;

    allocate(self->stack.memory, self->stack.max * sizeof(int));
    priv->_io.src = va_arg(*args, const char *);
}

static void vm_dtor(Object __cplus__unused *instance)
{
    /* __nothing__ */
}
