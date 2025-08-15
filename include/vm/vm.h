#ifndef C_PLUS_VM_H_
#define C_PLUS_VM_H_

#include <oop/interface.h>
#include <oop/macros.h>
#include <vm/types.h>

#include <vm/io.h>

struct _VMData {
    IOStream _io;
    const Inst *program;
};

typedef struct VM {
    const Class *class;

    void (*start)(struct VM *self);

    struct _VMData _priv;
} VM;

__cplus__const const Class *VMGetClass(void);
#define VMClass VMGetClass()

#endif /* C_PLUS_VM_H_ */
