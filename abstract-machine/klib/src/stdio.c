#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  for(int i = 0; fmt[i + 1] != '\0'; i ++){
    if(fmt[i] != '%'){
      char ch[2] = {fmt[i]};
      strcat(out,ch);
      continue;
    }
    else
    {
      char* ch;
      int d;
      switch (fmt[i + 1])
      {
      case '%':
        break;
      case 's':
        ch = va_arg(ap,char*);
        strcat(out,ch);
        break;
      case 'd':
        
        d = va_arg(ap, int);

        char chtemp[12] = {"+0000000000"};
        if(d < 0) chtemp[0] = '-';
        for(int i = 11; i > 0; i --){
          int temp = d % 10;
          if (temp < 0)temp = -temp;
          chtemp[i] = temp - '0';
          d = d / 10;
          if(d == 0) break;
        }
        ch = chtemp;
        for(int i = 1; i < 11; i ++){
          if(chtemp[i] != 0){
            chtemp[i - 1] = '-';
            ch = &chtemp[i - 1];
            break;
          }
        }
        strcat(out,ch);
        break;
      default:
        break;
      }
    }
    
  }
  
  //va_arg(ap,char);
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
