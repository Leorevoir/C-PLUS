#ifndef CPLUS_LD_HEADER_H_
#define CPLUS_LD_HEADER_H_

#include <cplus/types.h>

#define CPLUS_MAGIC_SIZE 5
#define CPLUS_MAGIC "CPLUS"
#define CPLUS_MAGIC_U8 {'C', 'P', 'L', 'U', 'S'}

#define CPLUS_VERSION_MAJOR 0x00
#define CPLUS_VERSION_MINOR 0x01

#ifndef __cplus__const
    #define __cplus__const __attribute__((const))
#endif

typedef enum {
    CPLUS_VERSION_0 = 0x00,
    CPLUS_VERSION_1 = 0x01,
    CPLUS_VERSION_2 = 0x02,
    CPLUS_VERSION_3 = 0x03,
    CPLUS_VERSION_4 = 0x04,
    CPLUS_VERSION_5 = 0x05,
    CPLUS_VERSION_6 = 0x06,
    CPLUS_VERSION_7 = 0x07,
    CPLUS_VERSION_8 = 0x08,
    CPLUS_VERSION_9 = 0x09,
} CPlusVersionNum;

typedef struct {
    CPlusVersionNum major;
    CPlusVersionNum minor;
} CPlusVersion;

typedef enum {
    CPLUS_ARCH_X86_64 = 0x01,
    CPLUS_ARCH_ARM = 0x02,
    CPLUS_ARCH_RISCV = 0x03,
} CPlusArch;

typedef enum {
    CPLUS_OFFSET_ENTRY_POINT,
    CPLUS_OFFSET_CODE,
    CPLUS_OFFSET_SYMBOLS,
    CPLUS_OFFSET_RELOCATIONS,
    CPLUS_OFFSET_DATA,
    CPLUS_OFFSET_COUNT
} CPlusOffset;

typedef struct {
    u8 magic[CPLUS_MAGIC_SIZE];
    CPlusVersion version;
    u32 arch;
    u32 program_size;
    u32 entry;
} CPlusHeader;

__cplus__const static inline const char *cplus_arch_to_string(CPlusArch arch)
{
    switch (arch) {
        case CPLUS_ARCH_X86_64:
            return "x86-64";
        case CPLUS_ARCH_ARM:
            return "ARM";
        case CPLUS_ARCH_RISCV:
            return "RISC-V";
        default:
            return "unknown";
    }
}

__cplus__const static inline const char *cplus_offset_to_string(CPlusOffset offset)
{
    switch (offset) {
        case CPLUS_OFFSET_ENTRY_POINT:
            return "entry point";
        case CPLUS_OFFSET_CODE:
            return "code";
        case CPLUS_OFFSET_SYMBOLS:
            return "symbols";
        case CPLUS_OFFSET_RELOCATIONS:
            return "relocations";
        case CPLUS_OFFSET_DATA:
            return "data";
        default:
            return "unknown";
    }
}

#endif /* CPLUS_LD_HEADER_H_ */
