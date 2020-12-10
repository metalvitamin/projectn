#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  int w = io_read(AM_GPU_CONFIG).width;  // TODO: get the correct width
  int h = io_read(AM_GPU_CONFIG).height;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = (inl(VGACTL_ADDR) >> 16) & 0xff, .height = inl(VGACTL_ADDR) & 0xff,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {

  int height = inl(VGACTL_ADDR) & 0xff;
  uint32_t *p = ctl->pixels;

  for(int i = 0; i < ctl->w; i ++){
    for(int j = 0; j < ctl->h; j ++){
      uint32_t position = height * (ctl->x + i) + (ctl->y + j);
      outl(FB_ADDR + position * 4, p[position]);
    }
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
