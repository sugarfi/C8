[org 0x7c00]
[bits 16]

mov si, mbr
mov cx, 4

find_active:
lodsb
test al, 0x80
jnz found_active
add si, 15

dec cx
jnz find_active

jmp no_active

found_active:
dec si
add si, 8
mov cx, 4
mov di, dap.dap_lba_start
rep movsb
mov cx, 2
mov di, dap.dap_lba_count
movsb
movsb
mov ah, 0x42
mov si, dap
int 0x13

mov ah, 0x00
mov al, 0x03
int 0x10

jmp 0x7e00

no_active:
mov si, no_active_msg
mov ah, 0x0e
print_msg:
lodsb
int 0x10
or al, al
jnz print_msg

cli
hlt

no_active_msg: db 'No active partition found', 0

dap:
db 0x10
db 0x00
.dap_lba_count:
dw 0
dw 0
dw 0x07e0
.dap_lba_start:
dq 0

times 440-($-$$) db 0

%include "src/boot/mbr.asm"

times 510-($-$$) db 0
dw 0xaa55 ; Boot signature

%include "src/boot/pmode.asm"
