#ifndef _C8_USER_MATH
#define _C8_USER_MATH

int pow(int a, int b) {
    int out;
    for (out = 1; b > 0; b--) {
        out *= a;
    }
    return out;
}

#endif
