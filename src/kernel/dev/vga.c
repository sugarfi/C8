#include <dev/vga.h>

// VGA size
const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;

// Cursor pos
int vga_x = 0;
int vga_y = 0;

char vga_color(char fg, char bg) {
    /*
     * Given a foreground and background color, generates a VGA color byte
    */
    return fg | bg << 4;
}

short vga_entry(char c, char color) {
    /*
     * Given a character and color byte, generates a 1-word VGA entry
    */
    return (short) c | (short) color << 8;
}

void vga_enable_cursor(char start, char end) {
    /*
     * Stolen from OSDev wiki. Enables the cursor, and sets the scanlines.
    */
	outb(0x3d4, 0x0a);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | start);

	outb(0x3d4, 0x0b);
	outb(0x3d5, (inb(0x3d5) & 0xe0) | end);
}

void vga_disable_cursor(void) {
    /*
     * Stolen from OSDev wiki. Hides the cursor.
    */
	outb(0x3d4, 0x0b);
	outb(0x3d5, 0x20);
}

void vga_move_cursor(int x, int y) {
    /*
     * Stolen from OSDev wiki. Moves the cursor to a given position.
    */

	int pos = y * VGA_WIDTH + x;

	outb(0x3d4, 0x0f);
	outb(0x3d5, (char) (pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (char) ((pos >> 8) & 0xff));
}

short vga_get_cursor_pos(void) {
    /*
     * Stole from OSDev wiki. Gets the cursor position.
    */

    short pos = 0;
    outb(0x3d4, 0x0f);
    pos |= inb(0x3d5);
    outb(0x3d4, 0x0e);
    pos |= ((short) inb(0x3d5)) << 8;
    return pos;
}

void vga_scroll(void) {
    /*
     * Scrolls the VGA framebuffer one line down.
    */

    int i;
    short* vga = (short*) 0xb8000;
    for (i = VGA_WIDTH; i < (VGA_WIDTH * VGA_HEIGHT) + VGA_WIDTH; i++) {
        vga[i - VGA_WIDTH] = vga[i]; // Move this entry back one line
        vga[i] = 0x0720; // Delete it
    }
    vga_y--; // Move back one line
    vga_x = 0;
}

void vga_putc(char c, short color) {
    /*
     * Puts a single character at the cursor position.
    */

    int index = vga_y * VGA_WIDTH + vga_x; // Get the position
    short* vga = (short*) 0xb8000;
    switch (c) {
        case '\n': // Don't print newlines, just move down one
            vga_x = 0;
            vga_y++;
            break;
        case '\r': // For carriage returns, move back to the start of the line
            vga_x = 0;
            break;
        case '\t': // For tabs, move ahead 4 spaces
            vga_x += 4;
            break;
        case '\0': // For null characters, do nothing
            break;
        default: // Otherwise, print normally
            vga[index] = vga_entry(c, color);
            vga_x++;
            break;
    }
    while (vga_y >= VGA_HEIGHT) {
        vga_scroll(); // Scroll until everything is visible
    }
    vga_move_cursor(vga_x, vga_y); // Move the cursor
}

void vga_puts(const char* s, short color) {
    /*
     * Puts a string at the cursor position.
    */

    int i = 0;
    while (s[i]) {
        vga_putc(s[i], color); // Put each character
        i++;
    }
}

/* These two functions are required to avoid global variable in a header file. */
void increment_vga_x(int i)
{
	vga_x += i;
}

void increment_vga_y(int i)
{
	vga_y += i;
}

int get_vga_x()
{
	return vga_x;
}

int get_vga_y()
{
	return vga_y;
}
