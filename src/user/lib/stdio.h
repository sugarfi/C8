#ifndef _C8_USER_STDIO
#define _C8_USER_STDIO

#include <math.h>
#include <string.h>
#include <stdarg.h>

void puts(char* s) {
    __asm__ __volatile__ ("int $0x80" : : "a" (1), "D" (1), "S" ((unsigned int) s));
}

void putc(char c) {
    char s[2];
    memcpy(s, &c, 1);
    __asm__ __volatile__ ("int $0x80" : : "a" (1), "D" (1), "S" ((unsigned int) s));
}

void putn(int i, int base) {
    if (i < 0) {
        putc('-');
        i = -i;
    }
    if (i == 0) {
        putc('0');
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
        putc(digits[this]);
        i -= this * pow(base, n);
        n--;
    }
}

void puti(int i) {
    putn(i, 10);
}

void putb(int i) {
    putn(i, 2);
}

void puth(int i) {
    putn(i, 16);
}

void puto(int i) {
    putn(i, 8);
}

void printf(char* format, ...) {
    va_list ap;
    int i;
    char spec;
    va_start(ap, format);
    for (i = 0; i < strlen(format); i++) {
        if (format[i] == '%') {
            spec = format[i + 1];
            if (spec == 's') {
                puts(va_arg(ap, char*));
            } else if (spec == 'd') {
                puti(va_arg(ap, int));
            } else if (spec == 'c') {
                putc((char) va_arg(ap, int));
            } else if (spec == 'x') {
                puth(va_arg(ap, int));
            } else if (spec == 'o') {
                puto(va_arg(ap, int));
            } else if (spec == 'b') {
                putb(va_arg(ap, int));
            } else if (spec == '%') {
                putc('%');
            }
            i++;
        } else {
            putc(format[i]);
        }
    }
}

void gets(char* out) {
    __asm__ __volatile__ ("int $0x80" : : "a" (0), "S" (0), "D" ((unsigned int) out));
}

#endif
