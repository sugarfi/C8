/*
 * dev/ps2.h - PS/2 Keyboard Driver
 * It contains functions to read from port 0x60, the PS/2 port,
 * and generate a keypress packet or sorts from that.
*/
#ifndef _C8_PS2
#define _C8_PS2

#include <sys/port.h>
#include <dev/vga.h>

typedef struct {
    char scancode;
    char ascii;
} ps2_key_t; // A mapping from scancode to ASCII

typedef struct {
    char ascii;
    char shift;
} ps2_shift_t; // A mapping from ASCII to shift-ASCII

#define PS2_ESC 0 // Special keys used by only this driver - in ASCII these are different
#define PS2_CTRL 1
#define PS2_SHIFT 2
#define PS2_CAPS 3
#define PS2_ALT 4
#define PS2_LEFT 5
#define PS2_RIGHT 6
#define PS2_UP 7
#define PS2_DOWN 8
#define PS2_SPECIAL_MAX 8 // Needed to tell if a key is special to the driver

#include <stdbool.h> // For booleans

typedef struct
{
    char scancode;
    char ascii;
    char type;
    bool shift;
    bool ctrl;
    bool alt;
    bool caps;
} ps2_keypress_t; // The actual keypress packet

char ps2_get_scancode(void);

char ps2_get_shift(char ascii);

char ps2_ascii(char scancode);

ps2_keypress_t ps2_get_key(void);

#endif
