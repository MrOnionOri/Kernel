#ifndef ISR_H
#define ISR_H

#include <stdint.h>

struct regs {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

void isr_install();
void irq_install();
void isr_handler(struct regs *r);
void irq_handler(struct regs *r);

extern void* irq_stub_table[16];

#endif
