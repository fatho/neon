#include "panic.h"
#include "stdint.h"
#include "stddef.h"

/**
 * Calculates a VGA color byte from fore- and background color.
 */
#define VGACOLOR(BG,FG) (((BG) << 4) | ((FG) & 0x0F))
/// the light color or blink bit
#define LIGHT_OR_BLINK 0x8
#define RED     0x4
#define WHITE   0xF

void kernel_panic(const char* msg) {
    volatile uint16_t* vga = ((volatile uint16_t*)0xB8000);
    uint16_t vgaoffset = 0;
    
    for(uint16_t i = 0; i < 80 * 25; i++) {
	vga[i] = VGACOLOR(WHITE,RED) << 8 | ' ';
    }
    
    while(*msg && vgaoffset < (25*80)) {
	switch(*msg) {
	case '\n':
	    vgaoffset = vgaoffset + 80 - vgaoffset%80;
	    break;
	default:
	    vga[vgaoffset] = *msg | VGACOLOR(WHITE,RED) << 8;
	    vgaoffset += 1;
	}
	msg++;
    }

    while(1) ;
}
