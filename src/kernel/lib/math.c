#include <lib/math.h>

int pow(int a, int b)
{
    int out;
    for (out = 1; b > 0; b--) {
        out *= a;
    }
    return out;
}

int oct2bin(unsigned char* str, int size)
{
    int n = 0;
    unsigned char* c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}