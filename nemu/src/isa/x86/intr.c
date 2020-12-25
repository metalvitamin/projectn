#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  //printf("IDTR = %u\n", cpu.IDTR.idt);
  rtl_push(s, &cpu.eflags.EFLAGS);
  vaddr_t access = cpu.IDTR.idt + NO * 8;
  printf("access addr = %u\n", access);
  *s0 = vaddr_read(access, 4);

  *s1 = vaddr_read(access + 4, 4);
  uint8_t valid = (*s1 >> 15) & 0x1;
  if (valid == 1){
    vaddr_t gate = (*s0 & 0xffff) |(*s1 & ~0xffff);
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
