#ifndef CPLUS_LD_CPO_H_
#define CPLUS_LD_CPO_H_

#define CPLUS_MAX_SYMBOL_SIZE 64

#include <ld/types.h>

typedef enum {
    CPO_SYMBOL_TYPE_FUNCTION = 0x01,
    CPO_SYMBOL_TYPE_VARIABLE = 0x02,
} CPOSymbolType;

typedef enum {
    CPO_RELOC_TYPE_ABSOLUTE = 0x01,
    CPO_RELOC_TYPE_RELATIVE = 0x02,
} CPORelocType;

typedef struct {
    char name[CPLUS_MAX_SYMBOL_SIZE];
    CPOSymbolType type;
    u32 offset;
    u32 scope;
    u32 ret_type;
} CPOSymbol;

typedef struct {
    char name[CPLUS_MAX_SYMBOL_SIZE];
    u32 offset;
    u32 type;
} CPOReloc;

#endif /* CPLUS_LD_CPO_H_ */
