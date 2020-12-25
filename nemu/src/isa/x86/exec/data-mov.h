static inline def_EHelper(mov) {
  operand_write(s, id_dest, dsrc1);

  print_asm_template2(mov);
}

static inline def_EHelper(push) {
  // TODO();
  rtl_push(s, ddest);

  print_asm_template1(push);
}

static inline def_EHelper(pop) {
  // TODO();
  rtl_pop(s, s0);
  operand_write(s, id_dest, s0);

  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {
  // TODO();
  if (s->isa.is_operand_size_16) {
    // TODO();
    rtl_lr(s, s0, R_SP, 2);
    rtl_lr(s, s1, R_AX, 2);
    rtl_push(s, s1);
    rtl_lr(s, s1, R_CX, 2);
    rtl_push(s, s1);
    rtl_lr(s, s1, R_DX, 2);
    rtl_push(s, s1);
    rtl_lr(s, s1, R_BX, 2);
    rtl_push(s, s1);
    rtl_push(s, s0);
    rtl_lr(s, s1, R_BP, 2);
    rtl_push(s, s1);
    rtl_lr(s, s1, R_SI, 2);
    rtl_push(s, s1);
    rtl_lr(s, s1, R_DI, 2);
    rtl_push(s, s1);
    // eax, ecx, edx, ebx, esp, ebp, esi, edi;
  }
  else {
    *s0 = cpu.esp;
    rtl_push(s, &cpu.eax);
    rtl_push(s, &cpu.ecx);
    rtl_push(s, &cpu.edx);
    rtl_push(s, &cpu.ebx);
    rtl_push(s, s0);
    rtl_push(s, &cpu.ebp);
    rtl_push(s, &cpu.esi);
    rtl_push(s, &cpu.edi);
    // rtl_push(s, &cpu.edi);
  }

  print_asm("pusha");
}

static inline def_EHelper(popa) {
  // TODO();
  if (s->isa.is_operand_size_16) {
    TODO();
  }
  else {
    rtl_pop(s, &cpu.edi);
    rtl_pop(s, &cpu.esi);
    rtl_pop(s, &cpu.ebp);
    rtl_pop(s, s0);
    rtl_pop(s, &cpu.ebx);
    rtl_pop(s, &cpu.edx);
    rtl_pop(s, &cpu.ecx);
    rtl_pop(s, &cpu.eax);
  }

  print_asm("popa");
}

static inline def_EHelper(leave) {
  // TODO();
  rtl_lr(s, s0, R_EBP, id_dest->width);
  rtl_sr(s, R_ESP, s0, id_dest->width);
  rtl_pop(s, s0);
  rtl_sr(s, R_EBP, s0, id_dest->width);

  print_asm("leave");
}

static inline def_EHelper(cltd) {
  if (s->isa.is_operand_size_16) {
    // TODO();
    rtl_lr(s, s0, R_AX, 2);
    if ((int16_t)(uint16_t)(*s0) < 0) {
      *s0 = 0xffff;
    }
    else {
      *s0 = 0x0000;
    }
    rtl_sr(s, R_DX, s0, 2);
  }
  else {
    // TODO();
    rtl_lr(s, s0, R_EAX, 4);
    if ((int32_t)(*s0) < 0) {
      *s0 = 0xffffffff;
    }
    else {
      *s0 = 0x00000000;
    }
    rtl_sr(s, R_EDX, s0, 4);
  }

  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
  if (s->isa.is_operand_size_16) {
    rtl_lr(s, s0, R_AL, 1);
    if ((int8_t)(uint8_t)(*s0) < 0) {
      *s0 = 0xff00 | *s0;
    }
    rtl_sr(s, R_AX, s0, 2);
  }
  else {
    rtl_lr(s, s0, R_AX, 2);
    if ((int16_t)(uint16_t)(*s0) < 0) {
      *s0 = 0xffff0000 | *s0;
    }
    rtl_sr(s, R_EAX, s0, 4);
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
  operand_write(s, id_dest, dsrc1);

  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);

  print_asm_template2(lea);
}

static inline def_EHelper(movsb) {
  *s0 = vaddr_read(cpu.esi, 1);
  *s1 = vaddr_read(cpu.edi, 1);
  vaddr_write(cpu.edi, *s0, 1);
  vaddr_write(cpu.esi, *s1, 1);
  cpu.esi = cpu.esi + 1;
  cpu.edi = cpu.edi + 1;

  print_asm("movsb");
}

static inline def_EHelper(movsd) {
  if (id_dest->width == 2) {
    *s0 = vaddr_read(cpu.esi, 2);
    *s1 = vaddr_read(cpu.edi, 2);
    vaddr_write(cpu.edi, *s0, 2);
    vaddr_write(cpu.esi, *s1, 2);
    cpu.esi = cpu.esi + 2;
    cpu.edi = cpu.edi + 2;
  }
  else if (id_dest->width == 4) {
    *s0 = vaddr_read(cpu.esi, 4);
    *s1 = vaddr_read(cpu.edi, 4);
    vaddr_write(cpu.edi, *s0, 4);
    vaddr_write(cpu.esi, *s1, 4);
    cpu.esi = cpu.esi + 4;
    cpu.edi = cpu.edi + 4;
  }
  
  print_asm(s->isa.is_operand_size_16 ? "movsw" : "movsd");
}