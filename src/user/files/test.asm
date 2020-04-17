org 0x1000

mov edi, 0xb8000
mov ax, 0x2800
or al, [string]
mov word [edi], ax
ret

string: db 'Hello'
