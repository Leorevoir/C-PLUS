#ifndef C_PLUS_INSTRUCTIONS_HEADER_H_
#define C_PLUS_INSTRUCTIONS_HEADER_H_

#include <vm/types.h>

/** stack instructions */
void inst_push(VMStack *stack, const int value);
int inst_pop(VMStack *stack);

/** arithmetic instructions */
void inst_add(VMStack *stack);
void inst_sub(VMStack *stack);
void inst_mul(VMStack *stack);
void inst_div(VMStack *stack);
void inst_mod(VMStack *stack);

/** comparison instructions */
void inst_eq(VMStack *stack);
void inst_neq(VMStack *stack);
void inst_gt(VMStack *stack);
void inst_lt(VMStack *stack);
void inst_gte(VMStack *stack);
void inst_lte(VMStack *stack);

/** control flow instructions */
void inst_jmp(VMStack *stack, const int offset);
void inst_jz(VMStack *stack, const int offset);
void inst_jnz(VMStack *stack, const int offset);
void inst_call(VMStack *stack, const int target);
void inst_ret(VMStack *stack);

/** core instructions */
void inst_halt(VMStack *stack);

#endif /* C_PLUS_INSTRUCTIONS_HEADER_H_ */
