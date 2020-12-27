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
  uint64_t phdraddr = ehdr.e_phoff;
  printf("type = %d\n",ehdr.e_type);
  printf("poffset = 0x%x, phnum = %d, ehsize = 0x%x\n",ehdr.e_phoff, ehdr.e_phnum,ehdr.e_ehsize);
  printf("vaddr = 0x%x\n", ehdr.e_entry);
  printf("phdraddr = 0x%x\n",phdraddr);
  int count = ehdr.e_phnum;
  Elf_Phdr *phdr[ehdr.e_phnum];ramdisk_read(phdr[0], phdraddr, sizeof(Elf_Phdr));
  for(int i = 0; i < count; i ++){
    putch('\n');putch('\n');putch('\n');
    if(phdr[0]->p_type == PT_LOAD){
      size_t buf[phdr[0]->p_filesz];
      void *entrance;entrance = (void *)phdr[0]->p_vaddr;
      ramdisk_read(buf, phdr[0]->p_offset, phdr[0]->p_filesz);
      printf("vaddr = %x\n", phdr[0]->p_vaddr);
      memcpy(entrance, buf, phdr[0]->p_filesz);
      size_t zero[phdr[0]->p_memsz - phdr[0]->p_filesz];
      memset(zero, 0, phdr[0]->p_memsz - phdr[0]->p_filesz);
      entrance += phdr[0]->p_filesz;
      memcpy(entrance ,zero , phdr[0]->p_memsz - phdr[0]->p_filesz);
      
    }
    phdraddr += sizeof(Elf_Phdr);
    
  }
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

