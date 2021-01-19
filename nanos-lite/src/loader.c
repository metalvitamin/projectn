#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ehdr;
  int fd = fs_open(filename, 0 ,0);
  fs_read(fd, &ehdr, sizeof(ehdr));
  uint64_t phdroffset = ehdr.e_phoff;
  Elf_Phdr phdr = {0};
  for(int i = 0; i < ehdr.e_phnum; i ++){
    fs_read(fd, &phdr, sizeof(phdr));
    phdroffset += sizeof(phdr);
    // printf("type = %d, offset = 0x%x, filesz = 0x%x\n", phdr.p_type,phdr.p_offset,phdr.p_filesz);
    if(phdr.p_type == PT_LOAD){
      uint8_t buf[phdr.p_filesz];
      fs_lseek(fd, phdr.p_offset, 0);
      fs_read(fd, buf, phdr.p_filesz);
      memcpy((void *)phdr.p_vaddr, buf, phdr.p_filesz);
      memset((void *)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
      fs_lseek(fd, phdroffset, 0);
    }
    
    
  }
  fs_close(fd);
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

