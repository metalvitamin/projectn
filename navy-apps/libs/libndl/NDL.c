#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval seconds = {.tv_sec = 0, .tv_usec = 0};
  gettimeofday(&seconds, NULL);
  return seconds.tv_sec * 1000 + seconds.tv_usec / 1000;
}

int open(const char *path, int flags, ...);
int NDL_PollEvent(char *buf, int len) {
  // printf("NDL\n");
  evtdev = open("/dev/events", 0);
  int ret = read(evtdev , buf, len);
  // printf("ret = %d\n",ret);
  close(evtdev);
  if(ret != 0 )return 1;
  else return 0;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
  else
  {
    char buf[30];
    evtdev = 3;
    read(evtdev, buf, 23);
    // printf("%s\n",buf);
    sscanf(buf,"WIDTH: %d \nHEIGHT: %d",&screen_w,&screen_h);
    // screen_w = 400;
    // screen_h = 300;
    printf("screen is %d*%d\n",screen_w,screen_h);
    screen_w = (screen_w - *w) / 2;
    screen_h = (screen_h - *h) / 2;
    printf("width = %d, height = %d\n",*w,*h);
  }
  
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  fbdev = open("/dev/fb", 0);
  printf("draw a %d*%d at (%d,%d) on the canva\n", w, h, x, y);
  
  lseek(fbdev,((screen_w + x) << 16) | (screen_h + y), 0);
  write(fbdev, pixels, (w << 16) | h);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
