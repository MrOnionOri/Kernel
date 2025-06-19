#include "ports.h"
#include "screen.h"
#include "irq.h"
#include <stdint.h>

#define SC_MAX 57
#define KB_DATA 0x60
#define ENTER_KEY 0x1C
#define BACKSPACE_KEY 0x0E

static char key_buffer[128];
static int buffer_index = 0;

const char sc_ascii[] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0,' '
};

void keyboard_handler() {
    print_string("[K]"); // Verifica si el IRQ1 está siendo llamado

    uint8_t scancode = inb(0x60);
    // uint8_t scancode = inb(KB_DATA);

    if (scancode > SC_MAX) return;

    char c = sc_ascii[scancode];

    if (c == '\n') {
        key_buffer[buffer_index] = 0;
        buffer_index = 0;
    } else if (c == '\b' && buffer_index > 0) {
        buffer_index--;
        print_string("\b \b");
    } else if (c >= 32 && buffer_index < 127) {
        key_buffer[buffer_index++] = c;
        char str[2] = {c, 0};
        print_string(str);
    }
}

void init_keyboard() {
    irq_install_handler(1, keyboard_handler);
}

void read_line(char* buffer, int max_len) {
    while (1) {
        if (key_buffer[0]) {
            for (int i = 0; i < max_len && key_buffer[i]; i++) {
                buffer[i] = key_buffer[i];
                if (key_buffer[i] == '\n') {
                    buffer[i] = '\0';
                    key_buffer[0] = 0;
                    return;
                }
            }
        }
    }
}
