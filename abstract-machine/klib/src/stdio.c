#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


static inline void dec_int(char* ch, char chtemp[],int d){
  
  if(d < 0) {chtemp[0] = '-';d = -d;}
  for(int i = 10; i > 0; i --){
    int temp = d % 10;
    chtemp[i] = temp + '0';
    d = d / 10;
    if(d == 0) {
      chtemp[i - 1] = chtemp[0];
      if(chtemp[0] == '+'){
        ch = &chtemp[i];
      }
      else {
        ch = &chtemp[i - 1];
      }
      break;
    }
  }
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
    else if(fmt[i + 1] != '\0')
    {
      char* ch;
      int d;
      char chtemp[12] = {"+0000000000"};
      switch (fmt[i + 1])
      {
      case '%':
        ch = "%";
        sec_stream(ch);
        break;
      case 's':
        ch = va_arg(ap,char*);
        sec_stream(ch);
        break;
      case 'd':
        ch = "";
        d = va_arg(ap, int);
        dec_int(ch,chtemp,d);
        sec_stream(ch);
        break;
      
      }
      
      i ++;
    }
    
  }
  va_end(ap);
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  putch('f');putch('u');putch('c');putch('k');putch('\n');
  *out = '\0';
  va_list ap;
  va_start(ap,fmt);
  for(int i = 0; fmt[i] != '\0'; i ++){
    if(fmt[i] != '%'){
      char ch[2] = {fmt[i]};
      strcat(out,ch);
    }
    else if(fmt[i + 1] != '\0')
    {
      char* ch;
      int d;
      char chtemp[12] = {"+0000000000"};
      switch (fmt[i + 1])
      {
      case '%':
        ch = "%";
        strcat(out,ch);
        break;
      case 's':
        ch = va_arg(ap,char*);
        strcat(out,ch);
        break;
      case 'd':
        ch = "";
        d = va_arg(ap, int);
        dec_int( ch, chtemp,d);
      
        strcat(out,ch);
        break;
      
      }
      
      i ++;
    }
    
  }
  sec_stream(out);
  va_end(ap);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
