#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void*, size_t, size_t);
size_t ramdisk_write(const void*, size_t, size_t);
static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ehdr;
  ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
  size_t phdraddr = ehdr.e_phoff;
  int count = ehdr.e_phnum;
  Elf_Phdr *phdr[ehdr.e_phnum];
  for(int i = 0; i < count; i ++){
    
    ramdisk_read(phdr[i], phdraddr, sizeof(Elf_Phdr));
    if(phdr[i]->p_type == PT_LOAD){
      size_t buf[phdr[i]->p_filesz];
      ramdisk_read(buf, phdr[i]->p_offset, phdr[i]->p_filesz);
      ramdisk_write(buf ,phdr[i]->p_vaddr, phdr[i]->p_vaddr + phdr[i]->p_filesz);
      size_t zero[phdr[i]->p_memsz - phdr[i]->p_filesz];
      ramdisk_write(zero ,phdr[i]->p_vaddr + phdr[i]->p_filesz, phdr[i]->p_vaddr + phdr[i]->p_memsz);
    }
  }
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

