#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

static size_t Foffset[sizeof(file_table)/sizeof(Finfo)] = {0};

size_t ramdisk_read(void*, size_t, size_t);
size_t ramdisk_write(const void*, size_t, size_t);

int fs_open(const char *pathname, int flags, int mode){
  for(int i = FD_FB; i < sizeof(file_table)/sizeof(Finfo); i ++)
    if(strcmp(file_table[i].name, pathname) == 0) {
      return i;
    }

  panic("you can't create a file");
}

size_t fs_read(int fd, void *buf, size_t len){
  if(fd < FD_FB) return 0;
  if(fd < sizeof(file_table)/sizeof(Finfo)){
    size_t maxlen = file_table[fd].size;
    size_t offset = file_table[fd].disk_offset + Foffset[fd];
    if(Foffset[fd] + len > maxlen) panic("your offset or len is out of bound!");
    ramdisk_read(buf, offset, len);
    return len;
  }
  else
  {
    panic("your fd is too large!");
    return -1;
  }
}

size_t fs_write(int fd, const void *buf, size_t len){
  if(fd == FD_STDIN) return 0;
  else if(fd < FD_FB){
    for(int i = 0; i < len; i ++)
      putch(*(char *)buf);
    return 0;
  }
  
  if(fd < sizeof(file_table)/sizeof(Finfo)){
    size_t maxlen = file_table[fd].size;
    size_t offset = file_table[fd].disk_offset + Foffset[fd];
    if(Foffset[fd] + len > maxlen) panic("your offset or len is out of bound!");
    ramdisk_write(buf, offset, len);
    return len;
  }
  else
  {
    panic("your fd is too large!");
    return -1;
  }
}

size_t fs_lseek(int fd, size_t offset, int whence){
  if(fd < FD_FB) return 0;
  switch (whence)
  {
  case SEEK_SET:
    Foffset[fd] = offset;
    break;
  case SEEK_CUR:
    Foffset[fd] += offset;
    break;
  // case SEEK_END:
  //   Foffset[fd] = offset + file_table[fd].size;
    break;
  default:
    Log("unsupported whence");
    return -1;
  }
  return Foffset[fd];
}

int fs_close(int fd){
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}
