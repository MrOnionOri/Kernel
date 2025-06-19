; Multiboot Header
; ========================
; Requerido por GRUB para arrancar el kernel
; ========================

[bits 32]
section .multiboot
align 4
multiboot_header:
    dd 0x1BADB002              ; magic number
    dd 0x00                    ; flags (0 = align modules on page boundaries)
    dd -(0x1BADB002 + 0x00)    ; checksum

section .text
global _start
extern kernel_main

_start:
    call kernel_main
.hang:
    jmp .hang


; boot.asm - Multiboot header para GRUB
[bits 32]
section .multiboot
align 4
    dd 0x1BADB002          ; magic number
    dd 0x00                ; flags
    dd - (0x1BADB002 + 0x00) ; checksum

section .text
global start
extern kernel_main

start:
    call kernel_main
.halt:
    jmp .halt
