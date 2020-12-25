#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  vaddr_t access = cpu.IDTR.idt + NO * cpu.IDTR.length / 256;

  uint32_t first = vaddr_read(access, 4);

  uint32_t last = vaddr_read(access + 4, 4);
  uint8_t valid = (last >> 15) & 0x1;
  if (valid == 1){
    vaddr_t gate = (first & 0xffff) |(last & ~0xffff);
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
