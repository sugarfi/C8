void main(void) {
    char* hello = "hello";
    __asm__ __volatile__ ("int $0x80" : : "a" (1), "d" (3), "D" (1), "S" ((int) hello)); // Nice
}
