#include <error/assert.h>
#include <memory/allocate.h>

#include <ld/linker.h>

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

__cplus__used static void linker_ctor(void *instance, va_list *args);
__cplus__used static void linker_dtor(void *instance);

static __inline void _allocate_cplus_buffer(Linker *self)
{
    ssize_t br = read(self->stream.fd, &self->header, sizeof(self->header));

    __assert(br == sizeof(self->header), "invalid header, failed to link");

    for (u8 i = 0; i < CPLUS_MAGIC_SIZE; ++i) {
        __assert(self->header.magic[i] == CPLUS_MAGIC[i], "invalid magic, failed to link");
    }

    __assert(self->header.version.major == CPLUS_VERSION_MAJOR && self->header.version.minor == CPLUS_VERSION_MINOR,
        "invalid version, failed to link");
    __assert(self->header.arch == CPLUS_ARCH_X86_64, "invalid architecture, failed to link");
    __assert(self->header.program_size > 0, "invalid program size, failed to link");

    self->stream.size = self->header.program_size;
    allocate(self->stream.buffer, self->stream.size);
    br = read(self->stream.fd, self->stream.buffer, self->stream.size);
    __assert(br == (ssize_t) self->stream.size, "failed to read program, failed to link");
}

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
    printf("linking program '%s'...\n", self->stream.filename);
    printf("%s__HEADER__%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);
    printf("%smagic:%s    %.2s\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, self->header.magic);
    printf("%sversion:%s  %d.%d\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, self->header.version.major, self->header.version.minor);
    printf("%sarch:%s     %s\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, cplus_arch_to_string(self->header.arch));
    printf("%sprogram size:%s %d bytes\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET, self->header.program_size);
    printf("%soffsets:%s\n", CPLUS_LOG_YELLOW, CPLUS_LOG_RESET);

    for (u8 i = 0; i < CPLUS_OFFSET_COUNT; ++i) {
        printf("    %s%-12s%s %d\n", CPLUS_LOG_YELLOW, cplus_offset_to_string(i), CPLUS_LOG_RESET, self->header.offsets[i]);
    }
    printf("%s__END__%s\n", CPLUS_LOG_MAGENTA, CPLUS_LOG_RESET);

    for (u32 i = 0; i < self->header.program_size; ++i) {
        printf("%02x ", self->stream.buffer[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
}

/**
* private
*/

static void linker_ctor(void *instance, va_list *args)
{
    Linker *self = (Linker *) instance;

    self->class = LinkerGetClass();
    self->link = linker_link;
    self->stream.filename = va_arg(*args, const char *);

    __assert(fstat(self->stream.fd, &self->stream.st) != -1, "failed to get file size");
    self->stream.fd = open(self->stream.filename, O_RDONLY);

#ifdef FD_CLOEXEC
    fcntl(self->stream.fd, F_SETFD, FD_CLOEXEC);
#endif

    __assert(self->stream.fd > 0, "file does not exists!");
    _allocate_cplus_buffer(self);
}

static void linker_dtor(void *instance)
{
    Linker *self = (Linker *) instance;

    if (self->stream.fd >= 0) {
        close(self->stream.fd);
        self->stream.fd = -1;
    }
}
