#include "cc.h"

static inline def_EHelper(test) {
  // TODO();  amazing!
  rtl_and(s, s0, ddest, dsrc1);
  rtl_update_ZFSF(s, s0, id_dest->width);
  *s0 = 0;
  rtl_set_OF(s, s0);
  rtl_set_CF(s, s0);

  print_asm_template2(test);
}

static inline def_EHelper(and) {
  // TODO();
  rtl_and(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  *s0 = 0;
  rtl_set_OF(s, s0);
  rtl_set_CF(s, s0);

  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  // TODO();
  rtl_xor(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  *s0 = 0;
  rtl_set_OF(s, s0);
  rtl_set_CF(s, s0);

  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  // TODO();
  rtl_or(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  *s0 = 0;
  rtl_set_OF(s, s0);
  rtl_set_CF(s, s0);

  print_asm_template2(or);
}

static inline def_EHelper(not) {
  // TODO();
  rtl_not(s, s0, ddest);
  operand_write(s, id_dest, s0);
  // !!! No flags affected

  print_asm_template1(not);
}

static inline def_EHelper(sar) {
  // TODO();   *****!!!!!!!!!!!!!!!!!!!!!!!!
  rtl_msb(s, s1, ddest, id_dest->width);
  if (*s1 == 1) {
    rtl_sext(s, ddest, ddest, id_dest->width);  // test
  }
  rtl_sar(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  // TODO();
  rtl_shl(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  // TODO();
  rtl_shr(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_update_ZFSF(s, s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

static inline def_EHelper(rol) {
  *s0 = *dsrc1;
  *s2 = *ddest;
  rtl_shli(s, s0, s0, 24);
  rtl_shri(s, s0, s0, 24);  // *****
  while(*s0){
    rtl_msb(s, s1, s2, id_dest->width);
    rtl_set_CF(s, s1);
    rtl_shli(s, s2, s2, 1);
    rtl_or(s, s2, s2, s1);
    *s0 = *s0 - 1;
  }
  operand_write(s, id_dest, s2);
  rtl_update_ZFSF(s, s2, id_dest->width);

  if(*dsrc1 == 1){
    rtl_msb(s, s2, s2, id_dest->width);
    rtl_xor(s, s0, s1, s2);
    rtl_set_OF(s, s0);
  }  
}

static inline def_EHelper(ror) {
  *s0 = *dsrc1;
  *s2 = *ddest;
  rtl_shli(s, s0, s0, 24);
  rtl_shri(s, s0, s0, 24);  // *****
  while(*s0){
    rtl_lsb(s, s1, s2, id_dest->width);
    rtl_set_CF(s, s1);
    rtl_shri(s, s2, s2, 1);
    rtl_shli(s, s1, s1, id_dest->width * 8 - 1);
    rtl_or(s, s2, s2, s1);
    *s0 = *s0 - 1;
  }
  operand_write(s, id_dest, s2);
  rtl_update_ZFSF(s, s2, id_dest->width);

  if(*dsrc1 == 1){
    *s1 = (*s2 >> (id_dest->width * 8 - 2));
    rtl_msb(s, s2, s2, id_dest->width);
    rtl_xor(s, s0, s1, s2);
    rtl_set_OF(s, s0);
  }  
}

static inline def_EHelper(shld) {    // Add!
  // dest:dest, src1: CL/imm8, src2: inbit(source)
  *s1 = *dsrc1 & 0x1f;
  if (*s1 != 0) {
    if (*s1 < id_dest->width * 8) {    // don't forget * 8
      rtl_shri(s, s2, ddest, id_dest->width * 8 - *s1);
      *s2 = *s2 & 0x1;
      rtl_set_CF(s, s2);
    }
    
    *s0 = *ddest;
    *s2 = *dsrc2;
    rtl_shli(s, s0, s0, *s1);
    rtl_shri(s, s2, s2, id_dest->width * 8 - *s1);
    rtl_or(s, s0, s0, s2);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    // unnecessary to update CF and OF in NEMU
  }

  print_asm_template3(shld);
}

static inline def_EHelper(shrd) {    // Add!
  // dest:dest, src1: CL/imm8, src2: inbit(source)
  *s1 = *dsrc1 & 0x1f;
  if (*s1 != 0) {
    if (*s1 < id_dest->width * 8) {    // don't forget * 8
      rtl_shri(s, s2, ddest, *s1 - 1);
      *s2 = *s2 & 0x1;
      rtl_set_CF(s, s2);
    }
    
    *s0 = *ddest;
    *s2 = *dsrc2;
    rtl_shri(s, s0, s0, *s1);
    rtl_shli(s, s2, s2, id_dest->width * 8 - *s1);
    rtl_or(s, s0, s0, s2);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    // unnecessary to update CF and OF in NEMU
  }

  print_asm_template3(shrd);
}

static inline def_EHelper(bsr) {    // Add!
  // dest:r16/r32, src1: r/m16/32
  *s1 = *dsrc1;
  if (*s1 == 0) {
    *s0 = 1;
    rtl_set_ZF(s, s0);
  }
  else {
    *s2 = id_src1->width * 8 - 1;
    *s0 = 0;
    rtl_set_ZF(s, s0);
    rtl_msb(s, s0, s1, id_src1->width);
    while (*s0 == 0) {
      *s2 = *s2 - 1;
      rtl_shli(s, s1, s1, 1);
      rtl_msb(s, s0, s1, id_src1->width);
    }
    operand_write(s, id_dest, s2);
  }
  
  print_asm_template2(bsr);
}

static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}