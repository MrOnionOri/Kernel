; boot.asm — incluye header Multiboot

[bits 32]
[extern kernel_main]

section .multiboot
align 4
    ; Multiboot header (GRUB lo requiere)
    dd 0x1BADB002        ; magic
    dd 0x00              ; flags
    dd -(0x1BADB002 + 0x00) ; checksum = -(magic + flags)

section .text
global _start
_start:
    call kernel_main
.hang:
    jmp .hang
