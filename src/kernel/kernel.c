#define VIDEO_MEMORY (char*)0xB8000
#define MAX_COLS 80
#define MAX_ROWS 25

void clear_screen() {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        video[i * 2] = ' ';      // carácter vacío
        video[i * 2 + 1] = 0x07; // color blanco sobre negro
    }
}


void print(const char *str) {
    volatile char *video = (volatile char*) 0xB8000;
    while (*str) {
        *video++ = *str++;
        *video++ = 0x07; // color blanco sobre negro
    }
}

void kernel_main() {
    clear_screen();
    print("Hola desde mi kernel!\n");
    while (1);
}
