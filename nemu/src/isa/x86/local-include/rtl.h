#ifndef __X86_RTL_H__
#define __X86_RTL_H__

#include <rtl/rtl.h>
#include "reg.h"

/* RTL pseudo instructions */

static inline def_rtl(lr, rtlreg_t* dest, int r, int width) {
  switch (width) {
    case 4: rtl_mv(s, dest, &reg_l(r)); return;
    case 1: rtl_host_lm(s, dest, &reg_b(r), 1); return;
    case 2: rtl_host_lm(s, dest, &reg_w(r), 2); return;
    default: assert(0);
  }
}

static inline def_rtl(sr, int r, const rtlreg_t* src1, int width) {
  switch (width) {
    case 4: rtl_mv(s, &reg_l(r), src1); return;
    case 1: rtl_host_sm(s, &reg_b(r), src1, 1); return;
    case 2: rtl_host_sm(s, &reg_w(r), src1, 2); return;
    default: assert(0);
  }
}

static inline def_rtl(push, const rtlreg_t* src1) {
  // esp <- esp - 4
  *s0 = *src1;
  reg_l(4) -= 4;
  rtl_sm(s, rz, reg_l(4), s0, 4);
  // M[esp] <- src1
}

static inline def_rtl(pop, rtlreg_t* dest) {
  // dest <- M[esp]
  rtl_lm(s, dest, rz, reg_l(4), 4);
  reg_l(4) += 4;
  // esp <- esp + 4
}

static inline def_rtl(is_sub_overflow, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 - src2)
  *dest = 0;
  if((*src1 >> (8*width-1)) & 0x1){
    if(!((*src2 >> (8*width-1)) & 0x1)){
      if(!((*res >> (8*width-1)) & 0x1)){
        *dest = 1;
      }
    }
  }
  else
  {
    if((*src2 >> (8*width-1)) & 0x1){
      if((*res >> (8*width-1)) & 0x1){
        *dest = 1;
      }
    }
  }
}

static inline def_rtl(is_sub_carry, rtlreg_t* dest,
    const rtlreg_t* src1, const rtlreg_t* src2) {
  // dest <- is_carry(src1 - src2)
  if(*src1 < *src2){
    *dest = 1;
  }
  else{
    *dest = 0;
  }
  
}

static inline def_rtl(is_add_overflow, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 + src2)
  *dest = 0;
  if((*src1 >> (8*width-1)) & 0x1){
    if((*src2 >> (8*width-1)) & 0x1){
      if(!((*res >> (8*width-1)) & 0x1)){
        *dest = 1;
      }
    }
  }
  else{
    if(!((*src2 >> (8*width-1)) & 0x1)){
      if((*res >> (8*width-1)) & 0x1){
        *dest = 1;
      }
    }
  }
  
}

static inline def_rtl(is_add_carry, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1) {
  // dest <- is_carry(src1 + src2)
  if(*res < *src1){
    *dest = 1;
  }
  else{
    *dest = 0;
  }
}
#define cpueflagsCF cpu.eflags.CF
#define cpueflagsOF cpu.eflags.OF
#define cpueflagsZF cpu.eflags.ZF
#define cpueflagsSF cpu.eflags.SF
#define def_rtl_setget_eflags(f) \
  static inline def_rtl(concat(set_, f), const rtlreg_t* src) { \
    concat(cpueflags,f) = *src; \
  } \
  static inline def_rtl(concat(get_, f), rtlreg_t* dest) { \
    *dest = concat(cpueflags,f); \
  }

def_rtl_setget_eflags(CF)
def_rtl_setget_eflags(OF)
def_rtl_setget_eflags(ZF)
def_rtl_setget_eflags(SF)

static inline def_rtl(update_ZF, const rtlreg_t* result, int width) {
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])

  rtl_msb(s,t0,result,width);
  
  if(*t0 == 0){
    cpueflagsZF = 1;
  }
  else
  {
    cpueflagsZF = 0;
  }
  assert(cpueflagsZF == 1 || cpueflagsZF == 0);
}

static inline def_rtl(update_SF, const rtlreg_t* result, int width) {
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  cpueflagsSF = (*result >> (8*width-1)) & 0x1;
  assert(cpueflagsSF == 1 || cpueflagsSF == 0);
}

static inline def_rtl(update_ZFSF, const rtlreg_t* result, int width) {
  rtl_update_ZF(s, result, width);
  rtl_update_SF(s, result, width);
}

#endif
