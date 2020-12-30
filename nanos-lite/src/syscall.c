#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case 0: halt(a[0]);break;    //SYS_exit    //???
    case 1: yield();break;    //SYS_yield
    case 4: if(a[1] == 1 || a[1] == 2)
              for(int i = 0; i < a[3]; i ++) 
                putch(*(char *)(a[2]++)); 
            break;            //SYS_write
    case 9: c->GPR1 = 0; break;  //SYS_brk
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
