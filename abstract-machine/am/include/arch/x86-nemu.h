#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  uintptr_t eax, edx, ecx, ebx, esp, ebp, esi, edi, eip, cs, eflags;
  void *cr3;
  int irq;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
