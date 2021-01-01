#include <am.h>
#include <nemu.h>
#include <string.h>
#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  /*int i;
  int w = io_read(AM_GPU_CONFIG).width ;  // TODO: get the correct width
  int h = io_read(AM_GPU_CONFIG).height;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);*/
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = inw(VGACTL_ADDR + 2), .height = inw(VGACTL_ADDR),
    .vmemsz = 0
  };

}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  short width = io_read(AM_GPU_CONFIG).width;
  short height = io_read(AM_GPU_CONFIG).height;
  uint32_t *p = (uint32_t *)ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (int j = 0; j < ctl->h && ctl->y + j < height; j ++) {
    memcpy(&fb[(ctl->y + j) * width + ctl->x], p, sizeof(uint32_t) * (ctl->w < width - ctl->x ? ctl->w : width - ctl->x));
    p += ctl->w;
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
