#ifndef CPLUS_CPA_CPA_H_
#define CPLUS_CPA_CPA_H_

#include <cplus/ast.h>
#include <cplus/io.h>

#include <oop/interface.h>

typedef struct CPAssembly {
    const Class *class;

    void (*assemble)(struct CPAssembly *self);

    AST *root;
    IOStream output;
    const char *src;
} CPAssembly;

__cplus__const const Class *CPAssemblyGetClass(void);
#define CPAssemblyClass CPAssemblyGetClass()

#endif /* CPLUS_CPA_CPA_H_ */
