#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  void *cr3;
  
  uintptr_t edi, esi, ebp, esp, ebx, edx, ecx, eax, eip, cs, eflags;
  int irq;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
