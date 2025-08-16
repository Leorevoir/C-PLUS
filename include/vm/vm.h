#ifndef C_PLUS_VM_H_
#define C_PLUS_VM_H_

#include <oop/interface.h>
#include <oop/macros.h>

#include <vm/header.h>
#include <vm/io.h>
#include <vm/types.h>

#define CPLUS_VM_DEFAULT_MAX_STACK_SIZE 1024

struct _VMData {
    IOStream _io;

    const CPlusHeader *_header;
    const Inst *_program;

    size_t _program_size;
};

struct VMStack {
    int *memory;
    uint32_t max;
    uint32_t size;
    int inst_count;
    int current;
};

typedef struct VM {
    const Class *class;

    void (*start)(struct VM *self);
    void (*show)(const struct VM *self);

    VMStack stack;
    struct _VMData _priv;
} VM;

__cplus__const const Class *VMGetClass(void);
#define VMClass VMGetClass()

#endif /* C_PLUS_VM_H_ */
