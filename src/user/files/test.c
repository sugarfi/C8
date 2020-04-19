void main(void) {
    short* vga = (short*) 0xb8000;
    char* s = "hello";
    vga[0] = 0x2800 | s[1];
    return;
}
