#include <dev/vga.h>

/*
 * All the cursor functions are taken from here: https://wiki.osdev.org/Text_Mode_Cursor
 */

void vga_enable_cursor(u8 start, u8 end) {
    /*
     * Enables the text mode cursor and sets the start and end scanlines.
     */
    port_outb(0x0a, 0x3d4);
    port_outb((port_inb(0x3d5) & 0xc0) | start, 0x3d5);
    port_outb(0x0b, 0x3d4);
    port_outb((port_inb(0x3d5) & 0xe0) | end, 0x3d5);
}

void vga_disable_cursor(void) {
    /*
     * Disable the text mode cursor.
     */
    port_outb(0x0a, 0x3d4);
    port_outb(0x20, 0x3d5);
}

void vga_set_cursor_pos(u16 x, u16 y) {
    /*
     * Move the cursor to a specific x and y.
     */
    u16 pos = y * VGA_WIDTH + x;

    port_outb(0x0f, 0x3d4);
    port_outb((u8) (pos & 0xff), 0x3d5);
    port_outb(0x0e, 0x3d4);
    port_outb((u8) ((pos >> 8) & 0xff), 0x3d5);
}

u16 vga_get_cursor_pos(void) {
    /*
     * Get the cursor position as one number.
     * To extract x and y:
     * x = pos % VGA_WIDTH
     * y = pos / VGA_WIDTH
     */
    u16 pos = 0;
    port_outb(0x0f, 0x3d4);
    pos |= port_inb(0x3d5);
    port_outb(0x0e, 0x3d4);
    pos |= ((u16) port_inb(0x3d5)) << 8;
    return pos;
}

void vga_put(char *s, u8 color) {
    /*
     * Writes a string to the VGA buffer at the current cursor position.
     */
    u16 *vga = (u16 *) 0xb8000;
    
    u16 pos = vga_get_cursor_pos(); // Get the cursor position to start printing at.
    u16 y = pos / VGA_WIDTH;
    u16 x = pos % VGA_WIDTH;

    char c;
    for (; *s; s++) {
        c = *s; // Load one character to print.
        if (c == '\n') {
            x = 0; // A \n moves down one line.
            y += 1;
        } else if (c == '\t') {
            x += 4; // A \t moves forward by 4.
        } else if (c == '\r') {
            x = 0; // A \r moves back to the first column.
        } else {
            vga[y * VGA_WIDTH + x] = c | (color << 8); // Any other characters are written to the VGA buffer.
            x++;
        }
    }

    vga_set_cursor_pos(x, y); // After all printing is done, we move the cursor to the new position.
}
