#ifndef __ARCH_H__
#define __ARCH_H__

/* PA3.1
 * Date: 2020/08/10
 */
struct _Context {
  struct _AddressSpace *as;
  uintptr_t edi, esi, ebp, esp, ebs, edx, ecx, eax;
  int irq;
  uintptr_t eip, cs, eflags;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
