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

static bool ps2_shift = 0; // Modifier key press statuses
static bool ps2_ctrl = 0;
static bool ps2_alt = 0;
static bool ps2_caps = 0;

ps2_key_t PS2_SCANCODES_1[] = // Scan code set 1 - for a US qwerty keyboard only!
{
    {0x01, PS2_ESC}, // Escape
    {0x02, '1'},
    {0x03, '2'},
    {0x04, '3'},
    {0x05, '4'},
    {0x06, '5'},
    {0x07, '6'},
    {0x08, '7'},
    {0x09, '8'},
    {0x0a, '9'},
    {0x0b, '0'},
    {0x0c, '-'},
    {0x0d, '='},
    {0x0e, '\b'}, // Backspace
    {0x0f, '\t'},
    {0x10, 'q'},
    {0x11, 'w'},
    {0x12, 'e'},
    {0x13, 'r'},
    {0x14, 't'},
    {0x15, 'y'},
    {0x16, 'u'},
    {0x17, 'i'},
    {0x18, 'o'},
    {0x19, 'p'},
    {0x1a, '['},
    {0x1b, ']'},
    {0x1c, '\n'},
    {0x1d, PS2_CTRL}, // Left control
    {0x1e, 'a'},
    {0x1f, 's'},
    {0x20, 'd'},
    {0x21, 'f'},
    {0x22, 'g'},
    {0x23, 'h'},
    {0x24, 'j'},
    {0x25, 'k'},
    {0x26, 'l'},
    {0x27, ';'},
    {0x28, '\''},
    {0x29, '`'},
    {0x2a, PS2_SHIFT}, // Left shift
    {0x2b, '\\'},
    {0x2c, 'z'},
    {0x2d, 'x'},
    {0x2e, 'c'},
    {0x2f, 'v'},
    {0x30, 'b'},
    {0x31, 'n'},
    {0x32, 'm'},
    {0x33, ','},
    {0x34, '.'},
    {0x35, '/'},
    {0x36, PS2_SHIFT}, // Right shift
    {0x38, PS2_ALT}, // Left alt
    {0x39, ' '},
    {0x3a, PS2_CAPS}, // Caps lock
    {0x3b, 0xf1}, // F1 Aren't I clever to use this format to represent function keys?
    {0x3c, 0xf2}, // F2
    {0x3d, 0xf3}, // F3
    {0x3e, 0xf4}, // F4
    {0x3f, 0xf5}, // F5
    {0x40, 0xf6}, // F6
    {0x41, 0xf7}, // F7
    {0x42, 0xf8}, // F8
    {0x43, 0xf9}, // F9
    {0x44, 0xfa}, // F10
    {0x57, 0xfb}, // F11
    {0x58, 0xfc}, // F12
    {0x4b, PS2_LEFT}, // Left arrow
    {0x4d, PS2_RIGHT}, // Left arrow
    {0x75, PS2_UP}, // Left arrow
    {0x72, PS2_DOWN}, // Left arrow
};

ps2_shift_t PS2_SHIFTS[] = // ASCII to shift-ASCII map - only needed for non letters
{
    {'`', '~'},
    {'1', '!'},
    {'2', '@'},
    {'3', '#'},
    {'4', '$'},
    {'5', '%'},
    {'6', '^'},
    {'7', '&'},
    {'8', '*'},
    {'9', '('},
    {'0', ')'},
    {'-', '_'},
    {'=', '+'},
    {'[', '{'},
    {']', '}'},
    {'\\', '|'},
    {';', ':'},
    {'\'', '"'},
    {',', '<'},
    {'.', '>'},
    {'/', '?'},
};

char ps2_get_scancode(void);

char ps2_get_shift(char ascii);

char ps2_ascii(char scancode);

ps2_keypress_t ps2_get_key(void);

#endif
