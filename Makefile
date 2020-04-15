.PHONY: all bin/kernel.bin bin/csh.o
.DEFAULT_GOAL = all

CC=gcc
LD=ld
CFLAGS=-ffreestanding -fno-pie -m32 -c -I src/kernel -I src/kernel/lib -nostdinc -nostdlib -std=c99 -O0
LDFLAGS=-T linker.ld -melf_i386
APPS=csh
CSOURCES=$(wildcard src/kernel/*.c src/kernel/dev/*.c src/kernel/fs/*.c src/kernel/int/*.c src/kernel/mem/*.c src/kernel/lib/*.c src/kernel/sys/*.c)
COBJECTS=$(CSOURCES:.c=.o)

bin/boot.bin: src/boot/boot.asm
	@mkdir bin -p
	nasm -f bin -o bin/boot.bin src/boot/boot.asm

%.o: %.asm
	@mkdir bin -p
	nasm -f elf32 $< -o bin/$(notdir $@) # Thanks, notdir.

%.o: %.c
	@mkdir bin -p
	$(CC) $< $(CFLAGS) -o bin/$(notdir $@)

%.o: %.asm
	@mkdir bin -p
	nasm -f elf32 $< -o bin/$(notdir $@)

bin/kernel.bin: $(COBJECTS) src/kernel/entry.asm
	$(LD) $(addprefix bin/, $(notdir $(COBJECTS))) $(LDFLAGS) bin/entry.o -o bin/kernel.bin # noice

$(APPS):
	@mkdir src/user/files/bin -p
	$(CC) $(CFLAGS) -static -I src/user -I src/user/lib -o src/user/files/bin/$@.o src/user/files/$@.c

clean:
	rm bin/*.o os.img bin/*.bin files.tar src/user/files/bin/*.o -f

all: bin/boot.bin bin/kernel.bin $(APPS)
	dd if=/dev/zero of=os.img bs=512 count=4096
	dd if=bin/boot.bin of=os.img conv=notrunc
	dd if=bin/kernel.bin of=os.img conv=notrunc seek=1
	tar -C src/user/files -f files.tar -c $$(ls src/user/files)
	dd if=files.tar of=os.img conv=notrunc oflag=seek_bytes seek=131072
