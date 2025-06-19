#include "lib/idt.h"
#include "lib/isr.h"
#include "lib/irq.h"
#include "lib/keyboard.h"
#include "lib/screen.h"
#include <string.h>

void shell_loop();

void kernel_main() {
    clear_screen();
    init_idt();         // define la IDT
    isr_install();      // interrupciones 0–31
    irq_install();      // IRQ0–IRQ15 → debe incluir IRQ1 (teclado)
    init_keyboard();    // conecta IRQ1 con keyboard_handler
    __asm__ __volatile__("sti");  // HABILITA interrupciones globales

    print_string("Bienvenido a mi kernel tipo bash\n");
    shell_loop();
}

void shell_loop() {
    char input[128];
    while (1) {
        print_string("> ");
        read_line(input, sizeof(input));
        print_string("\n");

        if (strcmp(input, "help") == 0) {
            print_string("Comandos disponibles:\n- help\n- clear\n- echo <msg>\n- shutdown\n");
        } else if (strcmp(input, "clear") == 0) {
            clear_screen();
        } else if (strncmp(input, "echo ", 5) == 0) {
            print_string(input + 5);
            print_string("\n");
        } else if (strcmp(input, "shutdown") == 0) {
            print_string("Apagando sistema (simulado)...\n");
            break;
        } else {
            print_string("Comando no reconocido. Escribe 'help'.\n");
        }
    }
}
