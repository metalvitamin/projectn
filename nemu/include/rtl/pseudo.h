#ifndef __RTL_PSEUDO_H__
#define __RTL_PSEUDO_H__

#ifndef __RTL_RTL_H__
#error "Should be only included by <rtl/rtl.h>"
#endif

/* RTL pseudo instructions */

static inline def_rtl(li, rtlreg_t* dest, const rtlreg_t imm) {
  rtl_addi(s, dest, rz, imm);
}

static inline def_rtl(mv, rtlreg_t* dest, const rtlreg_t *src1) {
  if (dest != src1) rtl_add(s, dest, src1, rz);
}

static inline def_rtl(not, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- ~src1
  *dest = ~(*src1);
}

static inline def_rtl(neg, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- -src1
  *dest = -(*src1);
}

static inline def_rtl(sext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- signext(src1[(width * 8 - 1) .. 0])
  if(width == 1){
    if((*src1 & 0xff) > 0x80)
      *dest = 0xffffff00 | (*src1 & 0xff);
    else
      *dest = 0 | (*src1 & 0xff);
  }
  else if(width == 2)
  {
    if((*src1 & 0xffff) > 0x8000)
      *dest = 0xffff0000 | (*src1 & 0xffff);
    else
      *dest = 0 | (*src1 & 0xffff);
  }
  
}

static inline def_rtl(zext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- zeroext(src1[(width * 8 - 1) .. 0])
  if(width == 1){
    *dest = 0 | (*src1 & 0xff);
  }
  else if(width == 2)
  {
    *dest = 0 | (*src1 & 0xffff);
  }
  

}

static inline def_rtl(msb, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- src1[width * 8 - 1]
  switch (width)
  {
  case 1:
    *dest = *src1 & 0xff;
    break;
  
  case 2:
    *dest = *src1 & 0xffff;
    break;
  case 4:
    *dest = *src1 & 0xffffffff;
    break;
  default:
    assert(0);
  }
}

#endif
