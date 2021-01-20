#include <common.h>
#include "syscall.h"
#include <sys/time.h>
#include <proc.h>
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

extern void naive_uload(PCB *, const char*);

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
    case SYS_read: c->GPR1 = fs_read(a[1],(void *)a[2],a[3]);break;     
    case SYS_write: c->GPR1 = fs_write(a[1],(void *)a[2],a[3]); break;     
    case SYS_close: c->GPR1 = fs_close(a[1]); break;              
    case SYS_lseek: c->GPR1 = fs_lseek(a[1],a[2],a[3]);break;     
    case SYS_brk: c->GPR1 = 0; break;
    case SYS_execve: naive_uload(NULL,(void *)a[1]); break;
    case SYS_gettimeofday: 
    {
      struct timeval *tm = (void *)a[1];
      uint64_t time = io_read(AM_TIMER_UPTIME).us;
      tm->tv_sec = time / 1000000;
      tm->tv_usec = time % 1000000;
      c->GPR1 = 0;break;
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
