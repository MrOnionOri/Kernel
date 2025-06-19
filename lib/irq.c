#include "irq.h"
#include "isr.h"
#include "idt.h"
#include "ports.h"

#define IDT_IRQ_BASE 32

void* irq_routines[16];

void irq_install_handler(int irq, void (*handler)()) {
    irq_routines[irq] = handler;
}

void irq_handler(struct regs* r) {
    void (*handler)() = irq_routines[r->int_no - IDT_IRQ_BASE];
    if (handler) handler();

    // enviar EOI
    if (r->int_no >= 40)
        outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void irq_install() {
    irq_remap();
    for (int i = 0; i < 16; i++) {
        idt_set_gate(IDT_IRQ_BASE + i, (uint32_t)irq_stub_table[i]);
    }
}
