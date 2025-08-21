#include <utils.h>

#include "cpa_utils.h"

__cplus__used static void cpassembly_ctor(void *instance, va_list *args);
__cplus__used static void cpassembly_dtor(void *instance);

// clang-format off
static const Class __cplus__used cpassembly_class = {
    .__size__ = sizeof(CPAssembly),
    .__name__ = "CPAssembly",
    .__ctor__ = cpassembly_ctor,
    .__dtor__ = cpassembly_dtor,
};
// clang-format on

/**
* public
*/

__cplus__const const Class *CPAssemblyGetClass(void)
{
    return &cpassembly_class;
}

static void cpassembly_assemble(CPAssembly *self)
{
    cpassembly_semantic_analysis(self->root);
    cpassembly_emit_header(self);
    io_stream_liberate(&self->output);
}

/**
* private
*/

static void cpassembly_ctor(void *instance, va_list *args)
{
    CPAssembly *self = (CPAssembly *) instance;

    self->class = CPAssemblyClass;
    self->assemble = cpassembly_assemble;
    self->root = va_arg(*args, AST *);
    self->src = va_arg(*args, const char *);
    self->output.filename = add_extension(self->src, ".cpa");
    self->output.buffer = NULL;

    io_stream_allocate(&self->output, "wb");
}

static void cpassembly_dtor(void *instance)
{
    io_stream_liberate(&((CPAssembly *) instance)->output);
}
