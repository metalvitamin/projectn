#include <common.h>
#include "syscall.h"

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case SYS_exit: halt(a[0]);break;        //???
    case SYS_yield: yield();break;    
    case SYS_open: c->GPR1 = fs_open((char *)a[1],a[2],a[3]); break;     
    case SYS_read: c->GPR1 = fs_read(a[1],(void *)a[2],a[3]); break;     
    case SYS_write: c->GPR1 = fs_write(a[1],(void *)a[2],a[3]); break;     
    case SYS_close: c->GPR1 = fs_close(a[1]); break;              
    case SYS_lseek: c->GPR1 = fs_lseek(a[1],a[2],a[3]);break;     
    case SYS_brk: c->GPR1 = 0; break;  
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
