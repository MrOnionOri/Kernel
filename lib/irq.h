#ifndef IRQ_H
#define IRQ_H

void irq_install();
void irq_install_handler(int irq, void (*handler)());

#endif
