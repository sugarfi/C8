section .entry

[bits 32]
mov ebp, 0x90000
mov esp, ebp

extern kmain
call kmain
jmp $
