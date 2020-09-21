#ifndef __ISA_X86_H__
#define __ISA_X86_H__

#include <common.h>


// memory
#define x86_IMAGE_START 0x100000
#define x86_PMEM_BASE 0x0

// reg

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
  union{
    struct {
      uint32_t _32;
      uint16_t _16;
      uint8_t _8[2];
    }gpr[8];
    struct {
      uint32_t EAX:32;
      uint16_t AX:16;
      uint8_t AH:8,AL:8; 
    };
    struct {
      uint32_t EDX:32;
      uint16_t DX:16;
      uint8_t DH:8,DL:8;
    };
    struct {
      uint32_t ECX:32;
      uint16_t CX:16;
      uint8_t CH:8,CL:8; 
    };
    struct {
      uint32_t EBX:32;
      uint16_t BX:16;
      uint8_t BH:8,BL:8; 
    };
    struct {
      uint32_t EBP:32;
      uint16_t BP:16; 
    };
    struct {
      uint32_t ESI:32;
      uint16_t SI:16; 
    };
    struct {
      uint32_t EDI:32;
      uint16_t DI:16; 
    };
    struct {
      uint32_t ESP:32;
      uint16_t SP:16; 
    };
  /* Do NOT change the order of the GPRs' definitions. */

  /* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
   * in PA2 able to directly access these registers.
   */
  rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;

  vaddr_t pc;
  };
} x86_CPU_state;

// decode
typedef struct {
  bool is_operand_size_16;
  uint8_t ext_opcode;
  const rtlreg_t *mbase;
  rtlreg_t mbr;
  word_t moff;
} x86_ISADecodeInfo;

#define suffix_char(width) ((width) == 4 ? 'l' : ((width) == 1 ? 'b' : ((width) == 2 ? 'w' : '?')))
#define isa_vaddr_check(vaddr, type, len) (MEM_RET_OK)
#define x86_has_mem_exception() (false)

#endif
