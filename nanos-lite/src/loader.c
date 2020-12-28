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

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ehdr;
  ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
  uint64_t phdraddr = ehdr.e_phoff;
  // printf("type = %d\n",ehdr.e_type);
  // printf("poffset = 0x%x, phnum = %d, ehsize = 0x%x\n",ehdr.e_phoff, ehdr.e_phnum,ehdr.e_ehsize);
  printf("vaddr = 0x%x\n", ehdr.e_entry);
  printf("phdraddr = 0x%x\n",phdraddr);
  Elf_Phdr phdr = {0};
  for(int i = 0; i < ehdr.e_phnum; i ++){
    putch('\n');putch('\n');putch('\n');
    ramdisk_read(&phdr, phdraddr, sizeof(Elf_Phdr));
    printf("type = %d\n", phdr.p_type);
    printf("offset = 0x%x, vaddr = 0x%x, paddr = 0x%x, filesize = 0x%x, memsize = 0x%x, flags = 0x%x, align = 0x%x\n"
          , phdr.p_offset, phdr.p_vaddr, phdr.p_paddr, phdr.p_filesz, phdr.p_memsz, phdr.p_flags, phdr.p_align);
    if(phdr.p_type == PT_LOAD){
      uint8_t buf[phdr.p_filesz];
      ramdisk_read(buf, phdr.p_offset, phdr.p_filesz);
      memcpy((void *)phdr.p_vaddr, buf, phdr.p_filesz);
      memset((void *)(phdr.p_vaddr + phdr.p_filesz) ,0 , phdr.p_memsz - phdr.p_filesz);
      
    }
    
    phdraddr += sizeof(phdr);
    
  }
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

