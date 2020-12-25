#include <monitor/difftest.h>

// #include "../intr.c"

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

static inline def_EHelper(lidt) {
  // TODO();
  if (id_dest->width == 4) {
    // cpu.IDTR.Limit = vaddr_read(id_dest->imm, 2);
    // cpu.IDTR.Base = vaddr_read(id_dest->imm + 2, 4);
    // cpu.IDTR.Limit = vaddr_read(*ddest, 2);
    // cpu.IDTR.Base = vaddr_read(*ddest + 2, 4);
    rtl_lr(s, s0, R_EAX, 4);
    cpu.IDTR.Limit = vaddr_read(*s0, 2);
    cpu.IDTR.Base = vaddr_read(*s0 + 2, 4);
    // cpu.IDTR.Limit = 1;
    // cpu.IDTR.Base = 1;
    printf("Base = %08x\n", cpu.IDTR.Base);
    printf("Limit = %08x\n", cpu.IDTR.Limit);
  }
  else {
    cpu.IDTR.Limit = vaddr_read(*ddest, 2);
    cpu.IDTR.Base = vaddr_read(*ddest + 2, 4) & 0x00ffffff;
  }  

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

extern void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr);

static inline def_EHelper(int) {
  // TODO();
  // raise_intr(s, id_dest->val, s->seq_pc);
  raise_intr(s, *ddest, s->seq_pc);

  print_asm("int %s", id_dest->str);

#ifndef __DIFF_REF_NEMU__
  difftest_skip_dut(1, 2);
#endif
}

static inline def_EHelper(iret) {
  // TODO();
  if (s->isa.is_operand_size_16) {
    TODO();
  }
  else {
    rtl_pop(s, s0);
  }
  rtl_pop(s, &cpu.cs);
  if (s->isa.is_operand_size_16) {
    TODO();
  }
  else {
    rtl_pop(s, &cpu.eflags.value);
  }
  rtl_j(s, *s0);

  print_asm("iret");

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(in) {
  // TODO();
  switch (id_dest->width) {
    case 4: {
      *s0 = pio_read_l(*dsrc1);
      break;
    }
    case 2: {
      *s0 = pio_read_w(*dsrc1);
      break;
    }
    case 1: {
      *s0 = pio_read_b(*dsrc1);
      break;
    }
  }
  operand_write(s, id_dest, s0);

  print_asm_template2(in);
}

static inline def_EHelper(out) {
  // TODO();
  switch (id_src1->width) {
    case 4: {
      pio_write_l(*ddest, *dsrc1);
      break;
    }
    case 2: {
      pio_write_w(*ddest, *dsrc1);
      break;
    }
    case 1: {
      pio_write_b(*ddest, *dsrc1);
      break;
    }
  }
  
  print_asm_template2(out);
}
