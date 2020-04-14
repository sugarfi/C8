#ifndef _C8_PS2
#define _C8_PS2

/*
dev/ps2.h - PS/2 Keyboard Driver
It contains functions to read from port 0x60, the PS/2 port,
and generate a keypress packet or sorts from that.
*/

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

typedef struct {
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

ps2_key_t PS2_SCANCODES_1[] = { // Scan code set 1 - for a US qwerty keyboard only!
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

ps2_shift_t PS2_SHIFTS[] = { // ASCII to shift-ASCII map - only needed for non letters
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

char ps2_get_scancode(void) {
    /*
    Reads a scancode from the PS/2 port.
    */
    while (!(inb(0x64) & 1)) { // Wait until the mouse data is gone
    }
    char key = inb(0x60); // Read 1 byte
    return key;
}

char ps2_get_shift(char ascii) {
    /*
    Given an ASCII keycode, gets it's shift value.
    */
    ps2_shift_t this;
    int i;
    if ((ascii >= 'a') && (ascii <= 'z')) { // For lowercase letters, just subtract 32.
        return ascii - 32;
    }
    for (i = 0; i < 101; i++) { // Otherwise, search the map for our key
        this = PS2_SHIFTS[i];
        if (this.ascii == ascii) {
            return this.shift;
        }
    }
    return ascii; // As a last resort, just return the key
}

char ps2_ascii(char scancode) {
    /*
    Converts a scancode to ASCII.
    */
    ps2_key_t this;
    int i;
    for (i = 0; i < 101; i++) { // Iterate over the map
        this = PS2_SCANCODES_1[i];
        if (this.scancode == scancode) { // We found our key
            if (ps2_shift && this.ascii > 32) {
                return ps2_get_shift(this.ascii); // Check for shift
            }
            if (ps2_caps) {
                if ((this.ascii >= 'a') && (this.ascii <= 'z')) {
                    return this.ascii - 32; // Capitalize letters is capslock is on
                }
            }
            return this.ascii; // Otherwise, return the ASCII
        }
    }
    return 0; // If no match was found, return null
}

ps2_keypress_t ps2_get_key(void) {
    /*
    Generates a keypress packet.
    */
    char scancode = ps2_get_scancode(); // Get a scancode
    bool type = scancode & 0x80; // Check if it is a keypress or release
    char ascii;
    if (type) { // If it is a release, mask it to a press
        ascii = ps2_ascii(scancode & (~0x80));
    } else {
        ascii = ps2_ascii(scancode);
    }
    if (ascii == PS2_CTRL) { // Set modifier keys correctly
        ps2_ctrl = !type;
    } else if (ascii == PS2_SHIFT) {
        ps2_shift = !type;
    } else if (ascii == PS2_ALT) {
        ps2_alt = !type;
    } else if (ascii == PS2_CAPS && type == 0) { // Only set capslock on press
        ps2_caps = !ps2_caps;
    }
    ps2_keypress_t packet = {scancode, ascii, type, ps2_shift, ps2_ctrl, ps2_alt, ps2_caps}; // Generate the packet
    return packet;
}

#endif
