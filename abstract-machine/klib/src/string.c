#include <klib.h>
#include <stdint.h>


#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if(s == NULL){
    return 0;
  }
  for(size_t i = 0;;i ++){
    if(s[i] == '\0')
      return i;
  }
  return 0;
}

char *strcpy(char* dst,const char* src) {
  if(dst == NULL || src == NULL){
    return NULL;
  }
  for(size_t i = 0;; i ++){
    dst[i] = src[i];
    if(src[i] == '\0')
      return dst;
  }
  
  return NULL;
}

char* strncpy(char* dst, const char* src, size_t n) {
  if(dst == NULL || src == NULL){
    return NULL;
  }
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
  if(dst == NULL || src == NULL){
    return NULL;
  }
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
  if(s1 == NULL || s2 == NULL) return 0;
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
  if(s1 == NULL || s2 == NULL) return 0;
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
  if(v == NULL) return NULL;
  unsigned char* p = (unsigned char*)v;
  for(size_t i = 0; i < n; i ++){
    *p = c;
    p += 1ul;
  }
  return v;
}

void* memmove(void* dst,const void* src,size_t n) {
  if(dst == NULL || src == NULL){
    return NULL;
  }
  char* pout = (char*)dst;
  char* pin = (char*)src;
  if(pin > pout){
    while(n--){
      *pout = *pin;
      pout += 1ul;
      pin += 1ul;
    }
  }
  else if(pin < pout){
    pout = pout + n - 1ul;
    pin = pin + n - 1ul;
    while(n--){
      *pout = *pin;
      pout -= 1ul;
      pin -= 1ul;
    }
  }
  
  return dst;
}

void* memcpy(void* out, const void* in, size_t n) {
  if(out == NULL || in == NULL){
    return NULL;
  }
  char* pout = (char*)out;
  char* pin = (char*)in;
  while(n--){
    *pout = *pin;
    pout += 1ul;
    pin += 1ul;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  if(s1 == NULL || s2 == NULL) return 0;
  for(size_t i = 0; i < n; i ++){
    unsigned char c1 = ((unsigned char*)s1)[i], c2 = ((unsigned char*)s2)[i];
    if(c1 > c2) return 1;
    else if(c1 < c2) return -1;
  }
  return 0;
}

#endif
