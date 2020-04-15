/*
 * vga.h - A simple VGA framebuffer driver
*/
#ifndef _C8_VGA
#define _C8_VGA

#include <sys/port.h>


char vga_color(char fg, char bg);

short vga_entry(char c, char color);

void vga_enable_cursor(char start, char end);

void vga_disable_cursor(void);

void vga_move_cursor(int x, int y);

short vga_get_cursor_pos(void);

void vga_scroll(void);

void vga_putc(char c, short color);

void vga_puts(const char* s, short color);

/* These two functions are required to avoid global variable in a header file. */
void set_vga_x(int x);

void set_vga_y(int y);

int get_vga_x();

int get_vga_y();

#endif
