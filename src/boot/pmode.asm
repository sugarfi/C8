in al, 0x70
or al, 0x80
out 0x70, al
cli
in al, 0x92
or al, 2
out 0x92, al
lgdt [gdtr]
mov eax, cr0
or al, 1
mov cr0, eax
jmp CODE_SEG:pmode_main

gdt_start:
; Null descriptor
dq 0
; Code segment
gdt_code:
dw 0xffff
dw 0
db 0
db 0x9a
db 0b11001111
db 0
; Data segment
gdt_data:
dw 0xffff
dw 0
db 0
db 0x92
db 0b11001111
db 0
gdt_end:

gdtr:
dw gdt_end - gdt_start - 1
dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[bits 32]
pmode_main:

mov ax, DATA_SEG
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

times 1024-($-$$) nop
