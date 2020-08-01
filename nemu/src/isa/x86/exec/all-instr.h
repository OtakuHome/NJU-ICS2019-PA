#include "cpu/exec.h"
// data-move.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);
make_EHelper(pop);

// prefix.c
make_EHelper(operand_size);

// special.c
make_EHelper(nop);
make_EHelper(inv);
make_EHelper(nemu_trap);


// control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);

// arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(cmp);
make_EHelper(inc);
make_EHelper(dec);
make_EHelper(adc);

// logic.c
make_EHelper(xor);
make_EHelper(and);
make_EHelper(setcc);
make_EHelper(test);
make_EHelper(or);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(shr);



