#include <monitor/difftest.h>

static inline def_EHelper(lidt) {
  *s0 = reg_l(0);
  cpu.IDTR.length = vaddr_read(*s0, 2);
  cpu.IDTR.idt = vaddr_read(*s0 + 2, 4);

  print_asm_template1(lidt);
}

static inline def_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src1->reg, 4), id_dest->reg);
}

static inline def_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src1->reg, reg_name(id_dest->reg, 4));

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(int) {
  void raise_intr(DecodeExecState*, uint32_t, vaddr_t);
  raise_intr(s, *ddest, s->seq_pc);

  print_asm("int %s", id_dest->str);

#ifndef __DIFF_REF_NEMU__
  difftest_skip_dut(1, 2);
#endif
}

static inline def_EHelper(iret) {
  rtl_pop(s, &s->jmp_pc);printf("pc = 0x%08x\n", s->jmp_pc);
  rtl_j(s, s->jmp_pc);
  s->is_jmp = 1;
  rtl_pop(s,s0);
  cpu.cs = *s0;
  rtl_pop(s,s0);
  cpu.eflags.EFLAGS = *s0;

  print_asm("iret");

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

static inline def_EHelper(in) {
  switch (id_dest->width)
  {
  case 1:
    *s0 = pio_read_b(*dsrc1);
    break;
  case 2:
    *s0 = pio_read_w(*dsrc1);
    break;
  case 4:
    *s0 = pio_read_l(*dsrc1);
    break;
  default:
    assert(0);
  }
  operand_write(s,id_dest,s0);
  print_asm_template2(in);
}

static inline def_EHelper(out) {
  switch (id_dest->width)
  {
  case 1:
    pio_write_b(*ddest,*dsrc1);
    break;
  case 2:
    pio_write_w(*ddest,*dsrc1);
    break;
  case 4:
    pio_write_l(*ddest,*dsrc1);
    break;
  default:
    assert(0);
  }

  print_asm_template2(out);
}
