#include <dev/ps2.h>

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
