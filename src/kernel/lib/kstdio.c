#include <lib/kstdio.h>

void kputs(char* s)
{
    vga_puts(s, 0x07);
}

void kputn(int i, int base)
{
    if (i < 0) {
        vga_putc('-', 0x07);
        i = -i;
    }
    if (i == 0) {
        vga_putc('0', 0x07);
    }
    int this;
    char* digits = "0123456789abcdef";
    int n = 0;
    while (pow(base, n) <= i) {
        n++;
    }
    n--;
    while (n > -1) {
        this = i / pow(base, n);
        vga_putc(digits[this], 0x07);
        i -= this * pow(base, n);
        n--;
    }
}

void kputi(int i)
{
    kputn(i, 10);
}

void kputb(int i)
{
    kputn(i, 2);
}

void kputh(int i)
{
    kputn(i, 16);
}

void kputo(int i)
{
    kputn(i, 8);
}

void kprintf(char* format, ...)
{
    va_list ap;
    int i;
    char spec;
    va_start(ap, format);
    for (i = 0; i < strlen(format); i++) {
        if (format[i] == '%') {
            spec = format[i + 1];
            if (spec == 's') {
                kputs(va_arg(ap, char*));
            } else if (spec == 'd') {
                kputi(va_arg(ap, int));
            } else if (spec == 'c') {
                vga_putc((char) va_arg(ap, int), 0x07);
            } else if (spec == 'x') {
                kputh(va_arg(ap, int));
            } else if (spec == 'o') {
                kputo(va_arg(ap, int));
            } else if (spec == 'b') {
                kputb(va_arg(ap, int));
            } else if (spec == '%') {
                vga_putc('%', 0x07);
            }
            i++;
        } else {
            vga_putc(format[i], 0x07);
        }
    }
}

void kgets(char* out)
{
    int index = 0;
    int max = 0;
    ps2_keypress_t this;
    while (1) {
        this = ps2_get_key();
        if (this.ascii == '\n' && index) {
            out[max] = '\0';
            vga_putc('\n', 0x07);
            return;
        } else if (this.ascii == '\b' && this.type == 0 && index > 0) {
            set_vga_x(get_vga_x() - 1);
            vga_putc(' ', 0x07);
            set_vga_x(get_vga_x() - 1);
            vga_move_cursor(get_vga_x(), get_vga_y());
            max--;
            index--;
        } else if (this.ascii == PS2_LEFT && this.type == 0 && index > 0) {
            set_vga_x(get_vga_x() - 1);
            index--;
            vga_move_cursor(get_vga_x(), get_vga_y());
        } else if (this.ascii == PS2_RIGHT && this.type == 0 && index < (max - 1)) {
            set_vga_x(get_vga_x() + 1);
            index++;
            vga_move_cursor(get_vga_x(), get_vga_y());
        } else if (this.type == 0 && this.ascii >= ' ') {
            out[index] = this.ascii;
            vga_putc(this.ascii, 0x07);
            index++;
            if (index >= (max - 1)) {
                max++;
            }
        }
    }
}
