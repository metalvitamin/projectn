#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  // TODO();
  // printf("NO = %x\n", NO);
  rtl_push(s, &cpu.eflags.value);
  cpu.eflags.IF = 0;
  // cpu.eflags.TF = 0;
  rtl_push(s, &cpu.cs);
  rtl_push(s, &ret_addr);
  // *s0 = vaddr_read(cpu.IDTR.Base + NO * 8, 2);         // OFFSET 15..0
  // *s1 = vaddr_read(cpu.IDTR.Base + NO * 8 + 6, 2);     // OFFSET 31..16
  *s0 = vaddr_read(cpu.IDTR.Base + NO * 8, 4) & 0x0000ffff;         // OFFSET 15..0
  *s1 = vaddr_read(cpu.IDTR.Base + NO * 8 + 4, 4) & 0xffff0000;     // OFFSET 31..16
  // *s0 = (*s1 << 16) | *s0;
  *s0 = *s1 | *s0;
  rtl_j(s, *s0);
}

void query_intr(DecodeExecState *s) {
  TODO();
}
