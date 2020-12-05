#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  for(int i = 0; i < 8; i ++){
    if(ref_r->gpr[i]._32 != cpu.gpr[i]._32){
      #ifdef DEBUG
      printf("wrong reg index %d = %x",i,ref_r->gpr[i]._32);
      #endif
      return false;
    }
  }
  if(ref_r->pc != cpu.pc){
    return false; 
  }
  return true;
}

void isa_difftest_attach() {
}
