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

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENTS, FD_FB};

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
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
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  [FD_EVENTS] = {"/dev/events", 0, 0, events_read, invalid_write},
  [FD_FB] = {"/dev/fb", 0, 0, invalid_read, fb_write},
#include "files.h"
};

static size_t Foffset[sizeof(file_table)/sizeof(Finfo)] = {0};

size_t ramdisk_read(void*, size_t, size_t);
size_t ramdisk_write(const void*, size_t, size_t);

int fs_open(const char *pathname, int flags, int mode){
  printf("open %s\n",pathname);
  
  for(int i = FD_EVENTS; i < sizeof(file_table)/sizeof(Finfo); i ++){
    if(strcmp("/proc/dispinfo", file_table[i].name) == 0) file_table[i].read = dispinfo_read;
    if(strcmp(file_table[i].name, pathname) == 0) {
      return i;
    }
  }
  panic("you can't create a file");
}

size_t fs_read(int fd, void *buf, size_t len){
  // printf("read ");
  if(file_table[fd].read != NULL) return file_table[fd].read(buf, file_table[fd].disk_offset, len);


  if(fd < sizeof(file_table)/sizeof(Finfo)){
    size_t maxlen = file_table[fd].size;
    size_t offset = file_table[fd].disk_offset + Foffset[fd];
    // printf("offset = %d, len = %d, size = %d\n",Foffset[fd], len,maxlen);
    if(Foffset[fd] + len > maxlen){
      len = maxlen - Foffset[fd];
      Log("cause the filesize is fixed, you can just read %d bytes", len);
    }
    ramdisk_read(buf, offset, len);
    Foffset[fd] += len;
    return len;
  }
  else
  {
    panic("your fd = %d is too large!", fd);
    return -1;
  }
}

size_t fs_write(int fd, const void *buf, size_t len){
  // printf("write ");
  if(file_table[fd].write != NULL) return file_table[fd].write(buf, file_table[fd].disk_offset, len);
  
  if(fd < sizeof(file_table)/sizeof(Finfo)){
    size_t maxlen = file_table[fd].size;
    size_t offset = file_table[fd].disk_offset + Foffset[fd];
    // printf("offset = %d, len = %d, size = %d\n",Foffset[fd], len,maxlen);
    if(Foffset[fd] + len > maxlen){
      len = maxlen - Foffset[fd];
      Log("cause the filesize is fixed, you can just read %d bytes", len);
    }
    ramdisk_write(buf, offset, len);
    Foffset[fd] += len;
    return len;
  }
  else
  {
    panic("your fd is too large!");
    return -1;
  }
}

size_t fs_lseek(int fd, size_t offset, int whence){
  // printf("lseek ");
  if(fd < FD_FB) return 0;
  switch (whence)
  {
  case SEEK_SET:
    Foffset[fd] = offset;
    break;
  case SEEK_CUR:
    Foffset[fd] += offset;
    break;
  case SEEK_END:
    Foffset[fd] = offset + file_table[fd].size;
    break;
  default:
    printf("unsupported whence\n");
    return -1;
  }
  return Foffset[fd];
}

int fs_close(int fd){
  // printf("\nclose\n");
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
  int fbsize = 400 * 300 * 4;
  file_table[FD_FB].size += fbsize;
  
}
