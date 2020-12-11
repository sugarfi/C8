dd 0xc8c8c8c8
dw 0

mbr:
; MBR entry #1
db 0x80 ; Active
db 0
dw 0
db 0xc8 ; The C8 kernel
db 0
dw 0
dd 1
dd 16 ; 8 KiB
; MBR entry #2
db 0x00 ; Not active
db 0
dw 0
db 0x04 ; FAT16
db 0
dw 0
dd 16
dd 16384 ; 8 KiB
; MBR entry #3
db 0x00 ; Not active
db 0
dw 0
db 0x83 ; Linux (ext2)
db 0
dw 0
dd 16399 
dd 16384 ; 8 KiB
; MBR entry #4 (unused)
times 16 db 0
