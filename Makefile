.DEFAULT_GOAL = all
.PHONY: src/user/files/bin/test.o

CC=gcc # change if needed
LD=ld
CFLAGS=-fno-stack-protector -mgeneral-regs-only -mno-red-zone -ffreestanding -fno-pie -m32 -c -I src/kernel -I src/kernel/lib -nostdinc -nostdlib -std=c99 -Os -fno-omit-frame-pointer -g
LDFLAGS=-T linker.ld -melf_i386
CSOURCES=$(shell find src/kernel -name '*.c')
COBJECTS=$(CSOURCES:.c=.o)

bin/boot.bin: src/boot/boot.asm src/boot/pmode.asm src/boot/mbr.asm
	@mkdir bin -p
	@echo "Assemble boot.asm"
	@nasm -f bin -o bin/boot.bin src/boot/boot.asm

%.o: %.asm
	@mkdir bin -p
	@echo "Assemble $<"
	@nasm -f elf32 $< -o bin/$(notdir $@) # Thanks, notdir.

%.o: %.c
	@mkdir bin -p
	@echo "Compile $<"
	@$(CC) $< $(CFLAGS) -o bin/$(notdir $@)

bin/kernel.bin: $(COBJECTS) src/kernel/entry.o
	@echo "Link"
	@$(LD) $(addprefix bin/, $(notdir $(COBJECTS))) $(LDFLAGS) bin/entry.o -o bin/kernel.bin # noice

clean:
	@echo "Clean"
	@rm bin/*.o os.img bin/*.bin files.tar src/user/files/bin/*.o -f

all: bin/boot.bin bin/kernel.bin
	@echo "Clear os.img"
	@dd if=/dev/zero of=os.img bs=1K count=16392
	@echo "Write boot.bin to os.img"
	@dd if=bin/boot.bin of=os.img conv=notrunc
	@echo "Write kernel.bin to os.img"
	@dd if=bin/kernel.bin of=os.img conv=notrunc seek=2
	@echo "Generate files.tar"
	@tar -cf files.tar files
	@echo "Write files.tar to os.img"
	@dd if=files.tar of=os.img conv=notrunc seek=65
