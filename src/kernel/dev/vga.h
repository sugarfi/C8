#ifndef _C8_VGA
#define _C8_VGA

#include <dev/port.h>
#include <types.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void vga_enable_cursor(u8, u8);
void vga_disable_cursor(void);
void vga_set_cursor_pos(u16, u16);
u16 vga_get_cursor_pos(void);
void vga_put(char *, u8);

#endif
