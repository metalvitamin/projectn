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
        
        d = va_arg(ap, int);
        if(d < 0) chtemp[0] = '-';
        for(int i = 10; i > 0; i --){
          int temp = d % 10;
          if (temp < 0) temp = -temp;
          chtemp[i] = temp + '0';
          d = d / 10;
          if(d == 0) break;
        }
        //putch(chtemp[0]);putch(chtemp[1]);putch(chtemp[2]);putch(chtemp[3]);putch(chtemp[4]);putch(chtemp[5]);putch(chtemp[6]);putch(chtemp[7]);putch(chtemp[8]);putch(chtemp[9]);putch(chtemp[10]);putch('\n');
        ch = chtemp;
        for(int i = 1; i < 11; i ++){
          //putch(chtemp[i]);putch('\n');
          if(chtemp[i] != '0'){
            chtemp[i - 1] = chtemp[0];
            if(chtemp[0] == '+'){
              //putch('+');
              //putch(chtemp[i]);
              ch = &chtemp[i];
            }
            else if (chtemp[0] == '-'){
              //putch('-');
              ch = &chtemp[i - 1];
            }
            break;
          }
        }
        //putch(*ch);putch('\n');
        strcat(out,ch);
        break;
      
      }
      
      i ++;
    }
    
  }
  //putch(out[0]);putch(out[1]);putch(out[2]);putch(out[3]);putch(out[4]);putch(out[5]);putch(out[6]);putch(out[7]);putch(out[8]);putch(out[9]);putch(out[10]);
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
