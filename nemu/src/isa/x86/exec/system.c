#include "cpu/exec.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr); 


make_EHelper(lidt) {
  // note that the docode funtion is gp7_E that will not load the val.
  // hence here should use id_dest->addr
  cpu.IDTR.limit = vaddr_read(id_dest->addr, 2);	
  cpu.IDTR.base = vaddr_read(id_dest->addr + 2, 4);	
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

  difftest_skip_ref();
}

make_EHelper(int) {
  rtl_push(&cpu.eflags.val);
  rtl_push(&cpu.cs);
  rtl_push(&decinfo.seq_pc);
  
  raise_intr(id_dest->val, decinfo.seq_pc);
  
  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

make_EHelper(in) {

  // the type of dest must be register

  switch (id_dest->width){
  	case 1:
  		s0 = pio_read_b(id_src->val);
  		break;
  	case 2:
  		s0 = pio_read_w(id_src->val);
  		break;
  	case 4:
  		s0 = pio_read_l(id_src->val);
  		break;
  	default:
		panic("Invalid width");
  }
  
  operand_write(id_dest, &s0);	// or rtl_sr
  
  print_asm_template2(in);
}

make_EHelper(out) {

  // the type of src must be register

  switch (id_src->width){
  	case 1:
  		pio_write_b(id_dest->val, id_src->val);
  		break;
  	case 2:
  		pio_write_w(id_dest->val, id_src->val);
  		break;
  	case 4:
		pio_write_l(id_dest->val, id_src->val);
		break;
	default:
		panic("Invalid width");
  }

  print_asm_template2(out);
}
