#include <isa.h>
#include <stdlib.h>
#include <time.h>
#include "local-include/reg.h"
#include "common.h" //Add types

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char regpc[] = "pc";

void reg_test() {
  srand(time(0));
  word_t sample[8];
  word_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
  int i;
  printf("pc:  0x%08x \n", reg_pc());
  for (i = R_EAX; i <= R_EDI; i++) {
    printf("%s: 0x%08x | ", regsl[i], reg_l(i));
    if (i % 4 == 3) {
      printf("\n");
    }
  }
  for (i = 0; i <= 70; i++) {
    printf("-");
  }
  printf("\n");

  for (i = R_AL; i <= R_BH; i++) {
    printf("%s:  0x%04x     | ", regsw[i], reg_w(i));
    if (i % 4 == 3) {
      printf("\n");
    }
  }
  for (i = 0; i <= 70; i++) {
    printf("-");
  }
  printf("\n");

  for (i = R_AX; i <= R_DI; i++) {
    printf("%s:  0x%02x       | ", regsb[i], reg_b(i));
    if (i % 4 == 3) {
      printf("\n");
    }
  }
  return;
}

word_t isa_reg_str2val(const char *s, bool *success) {
  *success = false;
  int len = strlen(s);
  if (len == 2) {
    if (strcmp(s, regpc) == 0) {
        *success = true;
        return cpu.pc;
    }
    else {
      for (int i = 0; i <= 7; i++) {
        if (strcmp(s, regsw[i]) == 0) {
          *success = true;
          return reg_w(i);
        }
        if (strcmp(s, regsb[i]) == 0) {
          *success = true;
          return reg_b(i);
        }
      }
    }
  }
  else if (len == 3) {
    for (int i = 0; i <= 7; i++) {
      if (strcmp(s, regsl[i]) == 0) {
        *success = true;
        return reg_l(i);
      }
    }
  }
  return 0;
}
