# Rutas
KEYBOARD_SRC := src/drivers/keyboard.c
KEYBOARD_OBJ := build/keyboard.o
SRC_DIR := src
BOOT_DIR := $(SRC_DIR)/boot
KERNEL_DIR := $(SRC_DIR)/kernel
INCLUDE_DIR := include
BUILD_DIR := build
ISO_DIR := iso

# Archivos fuente
BOOT_SRC := $(BOOT_DIR)/boot.asm
KERNEL_SRC := $(KERNEL_DIR)/kernel.c

BOOT_OBJ := $(BUILD_DIR)/boot.o
KERNEL_OBJ := $(BUILD_DIR)/kernel.o

KERNEL_BIN := $(BUILD_DIR)/kernel.bin
ISO_IMAGE := os-image.iso

# Compiladores
AS := nasm
CC := i686-elf-gcc
LD := i686-elf-ld

# Flags
ASFLAGS := -f elf
CFLAGS := -ffreestanding -I$(INCLUDE_DIR)
LDFLAGS := -T linker.ld

all: $(ISO_IMAGE)

$(KEYBOARD_OBJ): $(KEYBOARD_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_OBJ): $(BOOT_SRC) | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL_OBJ): $(KERNEL_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(KEYBOARD_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(ISO_IMAGE): $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $< $(ISO_DIR)/boot/kernel.bin
	echo 'set timeout=0\nmenuentry "MiKernel" { multiboot /boot/kernel.bin }' > $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

run: $(ISO_IMAGE)
	qemu-system-i386 -cdrom $<

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO_IMAGE)

.PHONY: all run clean
