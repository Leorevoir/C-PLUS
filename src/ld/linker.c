#include "oop/array.h"
#include <error/assert.h>
#include <memory/allocate.h>

#include <ld/linker.h>

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

__cplus__used static void linker_ctor(void *instance, va_list *args);
__cplus__used static void linker_dtor(void *instance);

// static __inline void _allocate_cplus_io_buffer(IOStream *)
// {
//     ssize_t br = read(self->stream.fd, &self->header, sizeof(self->header));
//
//     __assert(br == sizeof(self->header), "invalid header, failed to link");
//
//     for (u8 i = 0; i < CPLUS_MAGIC_SIZE; ++i) {
//         __assert(self->header.magic[i] == CPLUS_MAGIC[i], "invalid magic, failed to link");
//     }
//
//     __assert(self->header.version.major == CPLUS_VERSION_MAJOR && self->header.version.minor == CPLUS_VERSION_MINOR,
//         "invalid version, failed to link");
//     __assert(self->header.arch == CPLUS_ARCH_X86_64, "invalid architecture, failed to link");
//     __assert(self->header.program_size > 0, "invalid program size, failed to link");
//     __assert(self->header.entry < self->header.program_size, "invalid entry point, failed to link");
//
//     self->stream.size = self->header.program_size;
//     allocate(self->stream.buffer, self->stream.size);
//     br = read(self->stream.fd, self->stream.buffer, self->stream.size);
//     __assert(br == (ssize_t) self->stream.size, "failed to read program, failed to link");
// }

// static __inline void linker_debug_show(Linker *self)
// {
//     printf("%s__HEADER__      %s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
//     printf("%smagic:%s        %.2s\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, self->header.magic);
//     printf("%sversion:%s      %d.%d\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, self->header.version.major, self->header.version.minor);
//     printf("%sarch:%s         %s\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, cplus_arch_to_string(self->header.arch));
//     printf("%sprogram size:%s %d bytes\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, self->header.program_size);
//     printf("%sentry:%s        %u\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, self->header.entry);
//     printf("%s__END__%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
//
//     printf("\n%s__INST__%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
//     for (u32 i = 0; i < self->header.program_size; ++i) {
//         printf("%02x ", self->stream.buffer[i]);
//         if ((i + 1) % 16 == 0) {
//             printf("\n");
//         }
//     }
//     printf("%s__END__%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
// }

// clang-format off
static const Class __cplus__used linker_class = {
    .__size__ = sizeof(Linker),
    .__name__ = "Linker",
    .__ctor__ = linker_ctor,
    .__dtor__ = linker_dtor,
};
// clang-format on

__cplus__const const Class *LinkerGetClass(void)
{
    return &linker_class;
}

/**
* public
*/

static void linker_link(Linker *self)
{
    // linker_debug_show(self);
    (void) self;
}

/**
* private
*/

static __inline void linker_load_inputs(Linker *self, const Array *array)
{
    InputCPOFile *inputs = self->inputs;

    array_foreach(array, const char, it, {
        inputs->stream.filename = it;
        inputs->stream.fd = open(inputs->stream.filename, O_RDONLY);
        __assert(inputs->stream.fd >= 0, "failed to open input file: %s", inputs->stream.filename);
        __assert(fstat(inputs->stream.fd, &inputs->stream.st) == 0, "failed to stat input file: %s", inputs->stream.filename);
    });
}

static void linker_ctor(void *instance, va_list *args)
{
    Linker *self = (Linker *) instance;

    self->class = LinkerGetClass();
    self->link = linker_link;

    const Array *array = va_arg(*args, const Array *);
    self->output.stream.filename = va_arg(*args, const char *);
    self->flags = va_arg(*args, int);

    allocate(self->inputs, array->size(array));
    linker_load_inputs(self, array);
}

/** 
 * @brief close all input & output files because garbage collector doesn't handle it... yet!
 */
static void linker_dtor(void *instance)
{
    Linker *self = (Linker *) instance;

    for (u32 i = 0; i < self->inputs->size; ++i) {
        if (self->inputs->stream.fd >= 0) {
            close(self->inputs->stream.fd);
            self->inputs->stream.fd = -1;
        }
    }

    if (self->output.stream.fd >= 0) {
        close(self->output.stream.fd);
        self->output.stream.fd = -1;
    }
}
