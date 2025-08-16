#ifndef CPLUS_LD_LINKER_H_
#define CPLUS_LD_LINKER_H_

#include <oop/array.h>

#include <cplus/header.h>
#include <cplus/io.h>

typedef struct {
    CPlusHeader header;
    IOStream stream;
    u32 size;
} InputCPOFile;

typedef struct {
    IOStream stream;
} BinaryElfFile;

typedef struct Linker {
    const Class *class;

    void (*link)(struct Linker *self);

    InputCPOFile *inputs;
    BinaryElfFile output;
    int flags;
} Linker;

__cplus__const const Class *LinkerGetClass(void);
#define LinkerClass LinkerGetClass()

#endif /* CPLUS_LD_LINKER_H_ */
