#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
   Assert(NO <= cpu.IDTR.limit, "interrupt NO is outof bound.");
   uint32_t low = vaddr_read(cpu.IDTR.base + 8*NO, 4) & 0x0000ffff;
   uint32_t high = vaddr_read(cpu.IDTR.base + 8*NO + 4, 4) & 0xffff0000;
   rtl_j(high | low);
}

bool isa_query_intr(void) {
  return false;
}
