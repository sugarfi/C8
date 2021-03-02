section .entry

[bits 32]
mov ebp, 0x90000
mov esp, ebp
xor ebp, ebp
sti

extern kmain
call kmain
jmp $

extern idt_isr_handler
idt_isr_stub:
    cli
    pop eax 
    pop ebx
    pushad
    cld
    push ebx
    push eax
    call idt_isr_handler
    popad
    sti
    iret

%macro make_isr 2
    global idt_isr%1
    idt_isr%1:
        push byte %1
        %ifidn %2, 1
            push byte 0
        %endif
        jmp idt_isr_stub
%endmacro

make_isr 0,  0
make_isr 1,  0
make_isr 2,  0
make_isr 3,  0
make_isr 4,  0
make_isr 5,  0
make_isr 6,  0
make_isr 7,  0
make_isr 8,  1
make_isr 9,  0
make_isr 10, 1
make_isr 11, 1
make_isr 12, 1
make_isr 13, 1
make_isr 14, 1
make_isr 15, 0
make_isr 16, 0
make_isr 17, 1
make_isr 18, 0
make_isr 19, 0
make_isr 20, 0
make_isr 21, 0
make_isr 22, 0
make_isr 23, 0
make_isr 24, 0
make_isr 25, 0
make_isr 26, 0
make_isr 27, 0
make_isr 28, 0
make_isr 29, 0
make_isr 30, 1
make_isr 31, 0
