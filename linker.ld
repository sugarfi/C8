OUTPUT_FORMAT(binary);
SECTIONS {
    . = 0x7e00; /* Kernel starts at 0x7e00 */

    .text ALIGN (0x1000) : {
        *(.entry)
        *(.text)
    }
    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }
    .data ALIGN (0x1000) : {
        *(.data)
    }
    .bss ALIGN (0x1000) : {
        *(.bss)
    }
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.rel.dyn)
        *(.note.GNU-stack)
    }
}
