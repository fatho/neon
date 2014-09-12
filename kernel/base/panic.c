#include "panic.h"
#include "stdint.h"
#include "stddef.h"

#define VGA_STRIDE (2*80)

/**
 * Calculates a VGA color byte from fore- and background color.
 */
#define VGACOLOR(BG,FG) (((BG) << 4) | ((FG) & 0x0F))
/// the light color or blink bit
#define LIGHT_OR_BLINK 0x8
#define RED     0x4
#define WHITE   0xF

void kernel_panic(char* msg) {
  volatile uint16_t* vga = ((volatile uint16_t*)0xB8000);
  uint16_t vgaoffset = 0;

  while(*msg) {
    switch(*msg) {
      case '\n':
        vgaoffset = (vgaoffset + VGA_STRIDE) & VGA_STRIDE;
        break;
      default:
        vga[vgaoffset] = *msg | VGACOLOR(WHITE,RED) << 8;
        vgaoffset += 2;
    }
    msg++;
  }

  while(1) ;
}