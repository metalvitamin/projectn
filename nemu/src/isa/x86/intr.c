#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  //printf("IDTR = %u\n", cpu.IDTR.idt);
  printf("in cpu before:\n");
  printf("eflags = %d, cs = %d, eip = %d\n", cpu.eflags.EFLAGS, cpu.cs, s->seq_pc);
  rtl_push(s, &cpu.eflags.EFLAGS);
  rtl_push(s, &cpu.cs);
  rtl_push(s, &ret_addr);
  vaddr_t access = cpu.IDTR.idt + NO * 8;
  *s0 = vaddr_read(access, 4);

  *s1 = vaddr_read(access + 4, 4);
  uint8_t valid = (*s1 >> 15) & 0x1;
  if (valid == 1){
    vaddr_t gate = (*s0 & 0xffff) |(*s1 & ~0xffff);
    printf("gate = %x\n", gate);
    rtl_j(s,gate);
  }
  else
  {
    panic("INVALID INT NO!");
  }
  
}

void query_intr(DecodeExecState *s) {
  TODO();
}
