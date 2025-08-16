#ifndef CPLUS_LD_LINKER_H_
#define CPLUS_LD_LINKER_H_

#include <oop/interface.h>
#include <oop/macros.h>

#include <cplus/header.h>
#include <cplus/io.h>

typedef struct Linker {
    const Class *class;

    void (*link)(struct Linker *self);

    CPlusHeader header;
    IOStream stream;
} Linker;

__cplus__const const Class *LinkerGetClass(void);
#define LinkerClass LinkerGetClass()

#endif /* CPLUS_LD_LINKER_H_ */
