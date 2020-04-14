.PHONY: all bin/kernel.bin bin/csh.o

CC=gcc
LD=ld
CFLAGS=-ffreestanding -fno-pie -m32 -c -I src/kernel -I src/kernel/lib -nostdinc -nostdlib -std=c99 -O0
LDFLAGS=-T linker.ld -melf_i386
APPS=csh

bin/boot.bin: src/boot/boot.asm
	nasm -f bin -o bin/boot.bin src/boot/boot.asm

bin/kernel.o: src/kernel/kernel.asm
	nasm -f elf -o bin/kernel.o src/kernel/kernel.asm

bin/kernel.bin: src/kernel/kernel.c bin/kernel.o
	$(CC) $(CFLAGS) -o bin/kmain.o src/kernel/kernel.c
	$(LD) $(LDFLAGS) -o bin/kernel.bin bin/kernel.o bin/kmain.o

$(APPS):
	$(CC) $(CFLAGS) -static -I src/user -I src/user/lib -o src/user/files/bin/$@.o src/user/files/$@.c

all: bin/boot.bin bin/kernel.bin $(APPS)
	dd if=/dev/zero of=os.img bs=512 count=4096
	dd if=bin/boot.bin of=os.img conv=notrunc
	dd if=bin/kernel.bin of=os.img conv=notrunc seek=1
	tar -C src/user/files -f files.tar -c $$(ls src/user/files)
	dd if=files.tar of=os.img conv=notrunc oflag=seek_bytes seek=131072
