[bits 32]

extern kmain
call kmain
ret

global page_enable
page_enable:
    push ebp
    mov esp, esp
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    mov ebp, esp
    pop ebp
    ret

global page_load_dir
page_load_dir:
    push ebp
    mov ebp, esp
    mov eax, [esp + 8]
    mov cr3, eax
    mov esp, ebp
    pop ebp
    ret

global page_flush_tlb
page_flush_tlb:
    push ebp
    mov ebp, esp
    mov	eax, cr3
    mov	cr3, eax
    pop ebp
    ret

extern isr_handler
isr_base:
    push eax

    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    call isr_handler
    mov ebx, eax

    pop eax
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    pop eax
    add esp, 4
    mov eax, ebx
    sti
    iret

global isr_0
isr_0:
	cli
	push 0
	jmp isr_base

global isr_1
isr_1:
	cli
	push 1
	jmp isr_base

global isr_2
isr_2:
	cli
	push 2
	jmp isr_base

global isr_3
isr_3:
	cli
	push 3
	jmp isr_base

global isr_4
isr_4:
	cli
	push 4
	jmp isr_base

global isr_5
isr_5:
	cli
	push 5
	jmp isr_base

global isr_6
isr_6:
	cli
	push 6
	jmp isr_base

global isr_7
isr_7:
	cli
	push 7
	jmp isr_base

global isr_8
isr_8:
	cli
	push 8
	jmp isr_base

global isr_9
isr_9:
	cli
	push 9
	jmp isr_base

global isr_10
isr_10:
	cli
	push 10
	jmp isr_base

global isr_11
isr_11:
	cli
	push 11
	jmp isr_base

global isr_12
isr_12:
	cli
	push 12
	jmp isr_base

global isr_13
isr_13:
	cli
	push 13
	jmp isr_base

global isr_14
isr_14:
	cli
	push 14
	jmp isr_base

global isr_15
isr_15:
	cli
	push 15
	jmp isr_base

global isr_16
isr_16:
	cli
	push 16
	jmp isr_base

global isr_17
isr_17:
	cli
	push 17
	jmp isr_base

global isr_18
isr_18:
	cli
	push 18
	jmp isr_base

global isr_19
isr_19:
	cli
	push 19
	jmp isr_base

global isr_20
isr_20:
	cli
	push 20
	jmp isr_base

global isr_21
isr_21:
	cli
	push 21
	jmp isr_base

global isr_22
isr_22:
	cli
	push 22
	jmp isr_base

global isr_23
isr_23:
	cli
	push 23
	jmp isr_base

global isr_24
isr_24:
	cli
	push 24
	jmp isr_base

global isr_25
isr_25:
	cli
	push 25
	jmp isr_base

global isr_26
isr_26:
	cli
	push 26
	jmp isr_base

global isr_27
isr_27:
	cli
	push 27
	jmp isr_base

global isr_28
isr_28:
	cli
	push 28
	jmp isr_base

global isr_29
isr_29:
	cli
	push 29
	jmp isr_base

global isr_30
isr_30:
	cli
	push 30
	jmp isr_base

global isr_31
isr_31:
	cli
	push 31
	jmp isr_base

global isr_128
isr_128:
	cli
	push 128
	jmp isr_base
