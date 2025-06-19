ISO_NAME = os.iso

SRC = kernel.c \
      lib/screen.c \
      lib/keyboard.c \
      lib/ports.c \
      lib/string.c

OBJ = kernel.o \
      screen.o \
      keyboard.o \
      ports.o \
      string.o

all: iso

kernel.bin: asm/boot.asm kernel.c \
    lib/ports.c lib/screen.c lib/keyboard.c lib/string.c \
    lib/idt.c lib/irq.c lib/isr.c \
    asm/isr.asm asm/idt_flush.asm

	nasm -f elf32 asm/boot.asm -o obj/boot.o
	nasm -f elf32 asm/idt_flush.asm -o obj/idt_flush.o
	nasm -f elf32 asm/isr.asm -o obj/isr.o
	gcc  -m32 -ffreestanding -c kernel.c -o obj/kernel.o
	gcc  -m32 -ffreestanding -c lib/ports.c -o obj/ports.o
	gcc  -m32 -ffreestanding -c lib/screen.c -o obj/screen.o
	gcc  -m32 -ffreestanding -c lib/keyboard.c -o obj/keyboard.o
	gcc  -m32 -ffreestanding -c lib/string.c -o obj/string.o
	gcc  -m32 -ffreestanding -c lib/idt.c -o obj/idt.o
	gcc  -m32 -ffreestanding -c lib/irq.c -o obj/irq.o
	gcc  -m32 -ffreestanding -c lib/isr.c -o obj/isr_c.o
	ld.bfd -m elf_i386 -T linker.ld -o kernel.bin \
    	obj/boot.o obj/kernel.o obj/ports.o obj/screen.o obj/keyboard.o obj/string.o \
    	obj/idt.o obj/irq.o obj/isr_c.o obj/isr.o obj/idt_flush.o


iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	echo 'set timeout=0\nmenuentry "Mi OS" {\n multiboot /boot/kernel.bin\n boot\n}' > isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) isodir

clean:
	rm -rf *.o *.bin isodir $(ISO_NAME)
