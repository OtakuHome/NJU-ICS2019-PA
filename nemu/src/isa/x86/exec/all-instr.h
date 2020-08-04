#include "cpu/exec.h"
// data-move.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(movsx);
make_EHelper(leave);
make_EHelper(pop);
make_EHelper(cltd); // CWD (opcode 99)
make_EHelper(cwtl); // CBW (opcode 98)

// prefix.c
make_EHelper(operand_size);

// special.c
make_EHelper(nop);
make_EHelper(inv);
make_EHelper(nemu_trap);

// system.c
make_EHelper(out);
make_EHelper(in);

// control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(call_rm);
make_EHelper(jmp_rm);

// arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(cmp);
make_EHelper(inc);
make_EHelper(dec);
make_EHelper(adc);
make_EHelper(imul2);
make_EHelper(idiv); 
make_EHelper(imul1);
make_EHelper(mul);
make_EHelper(sbb);
make_EHelper(neg);
make_EHelper(div);
make_EHelper(imul3);

// logic.c
make_EHelper(xor);
make_EHelper(and);
make_EHelper(setcc);
make_EHelper(test);
make_EHelper(or);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(shr);
make_EHelper(not);


