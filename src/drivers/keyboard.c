#include <kernel.h>
#include <lib/io.h>
#include <stdint.h>

static char scancode_table[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', 0,
};

void print_hex(uint8_t sc) {
    char hex[] = "0123456789ABCDEF";
    char buf[3];
    buf[0] = hex[(sc >> 4) & 0xF];
    buf[1] = hex[sc & 0xF];
    buf[2] = '\0';
    print(buf);
}

char keyboard_read_char() {
    if ((inb(0x64) & 1) == 0)
        return 0;

    uint8_t sc = inb(0x60);

    // Mostrar scancode en decimal
    char num[4];
    num[0] = '0' + (sc / 100);
    num[1] = '0' + ((sc / 10) % 10);
    num[2] = '0' + (sc % 10);
    num[3] = '\0';

    print("SC: 0x");
    print_hex(sc);
    print(" (dec ");
    char dec[4];
    dec[0] = '0' + (sc / 100);
    dec[1] = '0' + ((sc / 10) % 10);
    dec[2] = '0' + (sc % 10);
    dec[3] = '\0';
    print(dec);
    print(")\n");

    if (sc > 127) {
        return 0;
    }

    char c = scancode_table[sc];

    print(" → ");
    if (c >= 32 && c <= 126) {
        print((char[]){c, 0});
    } else {
        print("[CTRL]");
    }
    print("\n");

    return c;
}



void init_keyboard() {
    // Este init es decorativo por ahora, en interrupciones sería útil
}
