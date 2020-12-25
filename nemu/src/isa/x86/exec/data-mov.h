static inline def_EHelper(mov) {
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
  rtl_push(s, ddest);

  print_asm_template1(push);
}

static inline def_EHelper(pop) {
  rtl_pop(s, ddest);

  operand_write(s,id_dest,ddest);
  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {
  printf("eax = %x, edx = %x, ecx = %x, ebx = %x, esp = %x, "
         "ebp = %x, esi = %x, edi = %x", cpu.eax, cpu.edx, 
         cpu.ecx, cpu.ebx, cpu.esp, cpu.ebp, cpu.esi, cpu.edi);
  if(s->isa.is_operand_size_16){
    *s0 = reg_l(4);
    rtl_push(s, &reg_l(0));
    rtl_push(s, &reg_l(1));
    rtl_push(s, &reg_l(2));
    rtl_push(s, &reg_l(3));
    rtl_push(s, s0);
    rtl_push(s, &reg_l(5));
    rtl_push(s, &reg_l(6));
    rtl_push(s, &reg_l(7));
  }
  else
  {
    *s0 = reg_w(4);
    *s1 = reg_w(0);
    rtl_push(s, s1);
    *s1 = reg_w(1);
    rtl_push(s, s1);
    *s1 = reg_w(2);
    rtl_push(s, s1);
    *s1 = reg_w(3);
    rtl_push(s, s1);
    rtl_push(s, s0);
    *s1 = reg_w(5);
    rtl_push(s, s1);
    *s1 = reg_w(6);
    rtl_push(s, s1);
    *s1 = reg_w(7);
    rtl_push(s, s1);
  }

  print_asm("pusha");
}

static inline def_EHelper(popa) {
  TODO();

  print_asm("popa");
}

static inline def_EHelper(leave) {
  //esp(sp) <- ebp(bp) then pop ebp(bp)

    reg_l(4) = reg_l(5);
    rtl_pop(s, &reg_l(5));

  print_asm("leave");
}

static inline def_EHelper(cltd) {
  if (s->isa.is_operand_size_16) {
    reg_w(2) = reg_w(0) < 0 ? -1u : 0;
  }
  else {
    reg_l(2) = reg_l(0) < 0 ? -1u : 0;
  }

  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
  if (s->isa.is_operand_size_16) {
    *s0 = reg_b(0);
    rtl_sext(s,s1,s0,1);
    reg_w(0) = *s1;
  }
  else {
    *s0 = reg_w(0);
    rtl_sext(s,s1,s0,2);
    reg_l(0) = *s1;
  }

  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movzx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  //rtl_zext(s, ddest, dsrc1, id_src1->width);
  //printf("\n\n\tsrc1 = %d\n\n",*dsrc1);
  //assert(*dsrc1 == *ddest);
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  
  operand_write(s, id_dest, ddest);
  
  print_asm_template2(lea);
 
}

static inline def_EHelper(movs) {
  reg_l(6) += id_dest->width;
  reg_l(7) += id_dest->width;
}
