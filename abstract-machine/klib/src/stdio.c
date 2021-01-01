#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define dec_xint(ch,chtemp,d,m){\
  for(int i = 10; i > 0; i --){\
    unsigned temp = d % 16;\
    chtemp[i] = temp > 9 ? temp - 10 + 'a' : temp + '0';\
    d = d / 16;\
    if(d == 0) {\
      if(m > 11 - i) i = 11 - m;\
      chtemp[i - 1] = chtemp[0];\
      ch = &chtemp[i];\
      break;\
    }\
  }\
}

#define dec_uint(ch,chtemp,d,m){\
  for(int i = 10; i > 0; i --){\
    unsigned temp = d % 10;\
    chtemp[i] = temp + '0';\
    d = d / 10;\
    if(d == 0) {\
      if(m > 11 - i) i = 11 - m;\
      chtemp[i - 1] = chtemp[0];\
      ch = &chtemp[i];\
      break;\
    }\
  }\
}

#define dec_int(ch,chtemp,d,m) {\
  uint32_t u = d;\
  if(d < 0) {chtemp[0] = '-';u = -d;}\
  for(int i = 10; i > 0; i --){\
    int temp = u % 10;\
    chtemp[i] = temp + '0';\
    u = u / 10;\
    if(u == 0) {\
      if(m > 11 - i) i = 11 - m;\
      chtemp[i - 1] = chtemp[0];\
      if(chtemp[0] == '+'){\
        ch = &chtemp[i];\
      }\
      else {\
        ch = &chtemp[i - 1];\
      }\
      break;\
    }\
  }\
}

#define format_choose(exec){\
      char* ch;\
      int d;\
      uint32_t u;\
      void *p;\
      int min_broad = 0;\
      char chtemp[12] = {"+0000000000"};\
format:\
      switch (fmt[++ i])\
      {\
      case '%':\
        ch = "%";\
        exec;\
        break;\
      case 'c':\
        ch = "0";\
        d = va_arg(ap, int);\
        ch[0] = d;\
        ch[1] = '\0';\
        exec;\
      case 's':\
        ch = va_arg(ap,char*);\
        exec;\
        break;\
      case 'd':\
        ch = "";\
        d = va_arg(ap, int);\
        dec_int(ch,chtemp,d,min_broad);\
        exec;\
        break;\
      case 'u':\
        ch = "";\
        u = va_arg(ap,unsigned);\
        dec_uint(ch,chtemp,u,min_broad);\
        exec;\
        break;\
      case 'x':\
        ch = "";\
        u = va_arg(ap,unsigned);\
        dec_xint(ch,chtemp,u,min_broad);\
        exec;\
        break;\
      case 'p':\
        ch = "0x";\
        exec;\
        p = va_arg(ap,void *);\
        u = (uint32_t)p;\
        dec_xint(ch,chtemp,u,min_broad)\
        exec;\
        break;\
      case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':\
        min_broad = min_broad * 10 + fmt[i] - '0';\
        goto format;\
      }\
    }

static inline void sec_stream(char* ch){
  for(int i = 0; ch[i] != '\0'; i ++){
    putch(ch[i]);
  }
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  for(int i = 0; fmt[i] != '\0'; i ++){
    if(fmt[i] != '%'){
      putch(fmt[i]);
    }
    else if(fmt[i + 1] != '\0') format_choose(sec_stream(ch))
    
  }
  va_end(ap);
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  *out = '\0';
  va_list ap;
  va_start(ap,fmt);
  for(int i = 0; fmt[i] != '\0'; i ++){
    if(fmt[i] != '%'){
      char ch[2] = {fmt[i]};
      strcat(out,ch);
    }
    else if(fmt[i + 1] != '\0') format_choose(strcat(out,ch))
    
  }

  va_end(ap);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  *out = '\0';
  va_list ap;
  va_start(ap,fmt);
  for(int i = 0; fmt[i] != '\0' && i < n; i ++){
    if(fmt[i] != '%'){
      char ch[2] = {fmt[i]};
      strcat(out,ch);
    }
    else if(fmt[i + 1] != '\0' && i != n - 1) format_choose(strcat(out,ch))
    
  }

  va_end(ap);
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
