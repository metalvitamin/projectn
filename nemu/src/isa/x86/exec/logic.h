#include "cc.h"


static inline def_EHelper(test) {
  TODO();
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  if(id_src1->width == 1 && id_dest->width != 1){
    int8_t temp = *dsrc1;
    int temp1 = temp;
    *dsrc1 = temp1;
  }
  rtl_and(s, ddest, ddest, dsrc1);
  rtl_set_CF(s,rz);
  rtl_set_OF(s,rz);
  rtl_update_ZFSF(s, ddest, id_dest->width);
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  rtl_xor(s, ddest, ddest, dsrc1);
  rtl_set_CF(s,rz);
  rtl_set_OF(s,rz);
  rtl_update_ZFSF(s, ddest, id_dest->width);
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  TODO();

  print_asm_template2(or);
}

static inline def_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}


static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

static inline def_EHelper(not) {
  TODO();

  print_asm_template1(not);
}

