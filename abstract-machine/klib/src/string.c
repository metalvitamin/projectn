#include <klib.h>
#include <stdint.h>


#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  for(size_t i = 0;;i ++){
    if(s[i] == '\0')
      return i;
  }
  return 0;
}

char *strcpy(char* dst,const char* src) {
  for(size_t i = 0;; i ++){
    dst[i] = src[i];
    if(src[i] == '\0')
      return dst;
  }
  
  return NULL;
}

char* strncpy(char* dst, const char* src, size_t n) {
  size_t i = 0;
  for(; i < n && src[i] != '\0'; i ++){
    dst[i] = src[i];
  }
  for(; i < n; i ++){
    dst[i] = '\0';
  }
  return dst;
}

char* strcat(char* dst, const char* src) {
  strcpy(dst+strlen(dst), src);
  size_t j;
  for(j = 0 ;; j ++){
    if(dst[j] == '\0')
      break;
  }
  for(size_t i = 0; ; i ++){
    dst[j + i] = src[i];
    if(src[i] == '\0')
      return dst;
  }
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  for(size_t i = 0;; i ++){
    if(s1[i] == '\0' && s2[i] == '\0') return 0; 
    else if(s1[i] == '\0') return -1;
    else if(s2[i] == '\0') return 1; 
    if(s1[i] > s2[i]) return 1;
    else if (s1[i] < s2[i]) return -1;
  }
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  for(size_t i = 0; i < n; i ++){
    if(s1[i] == '\0' && s2[i] == '\0') return 0; 
    else if(s1[i] == '\0') return -1;
    else if(s2[i] == '\0') return 1; 
    if(s1[i] > s2[i]) return 1;
    else if (s1[i] < s2[i]) return -1;
  }
  return 0;
}

void* memset(void* v,int c,size_t n) {
  putch('w');
  assert(0);
  for(size_t i = 0; i < n; i ++){
    ((unsigned char*)v)[i] = c;
  }
  return v;
}

void* memmove(void* dst,const void* src,size_t n) {
  putch('a');
  assert(0);
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  assert(0);
  putch('h');putch('i');
  putch('w');putch('h');putch('a');putch('t');
  for(size_t i = 0; i < n; i ++){
    *((unsigned char*)out + i) = *((unsigned char*)in + i);
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  assert(0);
  putch('i');
  for(size_t i = 0; i < n; i ++){
    unsigned char c1 = ((unsigned char*)s1)[i], c2 = ((unsigned char*)s2)[i];
    if(c1 > c2) return 1;
    else if(c1 < c2) return -1;
  }
  return 0;
}

#endif
