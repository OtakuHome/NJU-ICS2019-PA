#include "cpu/exec.h"
// data-move.c
make_EHelper(mov);
make_EHelper(push);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

// control.c
make_EHelper(call);

// arith.c
make_EHelper(sub);

// logic.c
make_EHelper(xor);
