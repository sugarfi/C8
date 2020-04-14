OUTPUT_FORMAT(binary);
SECTIONS {
    . = 0x10000;

    .text ALIGN (0x1000) : {
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
