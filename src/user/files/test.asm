section .text

mov edi, 0xb8000
mov ax, [string]
mov word [edi], ax

ret

section .data

string: dw 0x2841
