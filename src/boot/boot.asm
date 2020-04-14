[org 0x7c00]
[bits 16]

boot:

    mov cx, 10 ; The number of 128-sector groups to read

    read_loop:
        mov ah, 0x42 ; 0x42 is extended read
        mov si, dap ; Load our DAP
        int 0x13 ; Read the disk

        add word [dap.start], 0x80 ; Go to the next group
        add word [dap.segment], 0x1000 ; Increment the segment

        dec cx ; Decrement the count
        jnz read_loop ; Loop until it is 0

    mov ah, 0x00 ; Switch video mode
    mov al, 0x03 ; 0x03 is 80x25 test mode with colors
    int 0x10

    cli ; Disable interrupts

    in al, 0x92
    or al, 2
    out 0x92, al ; Fast A20 enable, non-boomer machines will support this

    lgdt [gdt_desc] ; Load our GDT

    mov eax, cr0
    or al, 1 ; Set the PE bit of cr0
    mov cr0, eax

    jmp 0x08:main ; Go to protected mode main

    cli ; Halt
    hlt ; We never get here anyway, so you could put whatever here

main:
    [bits 32] ; We are not in protected mode!

    mov ax, 0x10 ; Initialize the segments
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; Set up a stack
    mov esp, ebp

    call 0x10000 ; The kernel is at 0x10000

    jmp $ ; Hang
    ; We will never get here either, without some sort of segfault or something

dap:
    db 0x10 ; Size of DAP in bytes
    db 0x00 ; Unused
    .count:
        dw 0x0080 ; Number of sectors to read
    .offset:
        dw 0x0000 ; Offset to read to
    .segment:
        dw 0x1000 ; Segment to read to
    .start:
        dq 0x00000001 ; Starting LBA to read from

gdt_start:
    dd 0x00 ; Null descriptor
    dd 0x00
gdt_code:
    dw 0xffff ; Limit 0-15
    dw 0x0000 ; Base 0-15
    db 0x00 ; Base 16-23
    db 0x9a ; Type
    db 0xcf ; Flags and Limit 16-19
    db 0x00 ; Base 24-31
gdt_data:
    dw 0xffff ; Limit 0-15
    dw 0x0000 ; Base 0-15
    db 0x00 ; Base 16-23
    db 0x92 ; Type
    db 0xcf ; Flags and Limit 16-19
    db 0x00 ; Base 24-31
gdt_end:
gdt_desc:
    dw gdt_end - gdt_start - 1 ; Size - 1
    dd gdt_start ; Offset

times 510-($-$$) db 0 ; Pad to 512 bytes
dw 0xaa55 ; Boot signature
