#ifndef _C8_KSTDIO
#define _C8_KSTDIO

#include <dev/vga.h>
#include <dev/ps2.h>
#include <lib/math.h>
#include <lib/string.h>
#include <stdarg.h>

void kputs(char* s);

void kputn(int i, int base);

void kputi(int i);

void kputb(int i);

void kputh(int i);

void kputo(int i);

void kprintf(char* format, ...);

void kgets(char* out);

char kgetchar();

#endif
