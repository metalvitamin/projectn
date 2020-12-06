#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  int temp = inw(KBD_ADDR);

  kbd->keycode = temp ^ KEYDOWN_MASK;
  kbd->keydown = temp ^ kbd->keycode;
}
