#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"
#include "memory/vaddr.h"

#define check(name) ((*ref_r).name == cpu.name) 
#define reg_name(name) (*ref_r).name, cpu.name
#define print(regname) if(!check(regname)){ \
    printf("Problem in %s\n", #regname); \
    printf("Qemu:0x%x \nNemu:0x%x \n\n", reg_name(regname)); \
    return false; \
  }

#define check_f(name) ((*ref_r).eflags.name == cpu.eflags.name)
#define reg_f_name(name) (*ref_r).eflags.name, cpu.eflags.name
#define print_f(regname) if(!check_f(regname)){ \
    printf("Problem in %s\n", #regname); \
    printf("Qemu:0x%x \nNemu:0x%x \n\n", reg_name(regname)); \
    return false; \
  }

static int count = 0;
// static vaddr_t now = 0;
// static vaddr_t srore[100];
// static int index = 0;

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  print(eax) print(ecx) print(edx) print(ebx)
  print(esp) print(ebp) print(esi) print(edi)
  // print_f(CF) print(ZF) print(SF) print(IF) print(OF)

  // if((*ref_r).eflags.value != cpu.eflags.value){
  //   printf("Problem in eflags!\n");
  //   printf("In qemu:0x%x\nIn nemu:0x%x\n", (*ref_r).eflags.value, cpu.eflags.value);
  //   // return false;
  // }
  // 10bf14
  // now = vaddr_read(0x0010bf14, 4);
  // printf("0x0010bf14 = %08x\n", now);
  // if (now == 0x000000ff) {
    // printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
    // isa_reg_display();
    // printf("Eflags in Nemu:\n");
    // printf("CF:0x%x  ZF:0x%x  SF:0x%x  IF:0x%x  OF:0x%x\n\n", cpu.eflags.CF, cpu.eflags.ZF, cpu.eflags.SF, cpu.eflags.IF, cpu.eflags.OF);
    // // if(cpu.pc != (*ref_r).pc){
    //   printf("Problem in pc\n");
    //   printf("Qemu:0x%x \nNemu:0x%x \n\n", (*ref_r).pc, cpu.pc);
    //   return false;
    // }
    // vaddr_write(0x0010bf14, 0xffffffff, 4);
  // }
  // if (cpu.pc == 0x10044c) {
    // printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
    // isa_reg_display();
    // printf("Eflags in Nemu:\n");
    // printf("CF:0x%x  ZF:0x%x  SF:0x%x  IF:0x%x  OF:0x%x\n\n", cpu.eflags.CF, cpu.eflags.ZF, cpu.eflags.SF, cpu.eflags.IF, cpu.eflags.OF);
    // // if(cpu.pc != (*ref_r).pc){
    //   printf("Problem in pc\n");
    //   printf("Qemu:0x%x \nNemu:0x%x \n\n", (*ref_r).pc, cpu.pc);
      // return false;
    // }
    // vaddr_write(0x0010bf14, 0xffffffff, 4);
  // }
  //   if (cpu.pc == 0x100450) {
  //   printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
  //   isa_reg_display();
  //   printf("Eflags in Nemu:\n");
  //   printf("CF:0x%x  ZF:0x%x  SF:0x%x  IF:0x%x  OF:0x%x\n\n", cpu.eflags.CF, cpu.eflags.ZF, cpu.eflags.SF, cpu.eflags.IF, cpu.eflags.OF);
  //   // if(cpu.pc != (*ref_r).pc){
    //   printf("Problem in pc\n");
    //   printf("Qemu:0x%x \nNemu:0x%x \n\n", (*ref_r).pc, cpu.pc);
      // return false;
    // }
    // vaddr_write(0x0010bf14, 0xffffffff, 4);
  // }
  // if (cpu.pc == 0x100d0c) {
  //   printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
  //   return false;
  // }

  vaddr_t now;
  now = vaddr_read(0x00134F70, 4);
  // printf("0x00134f70 = %08x\n", now);
  if (now == 0x134f70) {
    printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
    isa_reg_display();
    // return false;
  }
  if (cpu.pc == 0x100a2a) {
    printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
    isa_reg_display();
    // return false;
  }
    if (cpu.pc == 0x100a7a) {
    printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
    isa_reg_display();
    // return false;
  }
    if (cpu.pc == 0x100a7b) {
    printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
    isa_reg_display();
    // return false;
  }

  now = vaddr_read(cpu.ebp + 8, 4);
  printf("ebp + 8 = %08x\n", now);
  // if (cpu.pc == 0x100721) {
  //   printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
  //   isa_reg_display();
  //   // return false;
  // }
  count++;
  // printf("Instruction %d, pc: 0x%08x \n", count, cpu.pc);
  // isa_reg_display();
  // printf("Eflags in Nemu:\n");
  // printf("CF:0x%x  ZF:0x%x  SF:0x%x  IF:0x%x  OF:0x%x\n\n", cpu.eflags.CF, cpu.eflags.ZF, cpu.eflags.SF, cpu.eflags.IF, cpu.eflags.OF);
  if(cpu.pc != (*ref_r).pc){
    printf("Problem in pc\n");
    printf("Qemu:0x%x \nNemu:0x%x \n\n", (*ref_r).pc, cpu.pc);
    return false;
  }
  return true;
}

void isa_difftest_attach() {
}
