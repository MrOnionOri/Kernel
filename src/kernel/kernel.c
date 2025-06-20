#include <stdint.h>
#include <kernel.h>
#include <drivers/keyboard.h>

#define VIDEO_MEMORY (char*)0xB8000
#define MAX_COLS 80
#define MAX_ROWS 25

int cursor_row = 0;
int cursor_col = 0;

void clear_screen() {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x07;
    }
}

void print_char(char c) {
    volatile char* video = (volatile char*)VIDEO_MEMORY;

    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        int offset = 2 * (cursor_row * MAX_COLS + cursor_col);
        video[offset] = c;
        video[offset + 1] = 0x07;
        cursor_col++;

        if (cursor_col >= MAX_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= MAX_ROWS) {
        for (int row = 1; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLS; col++) {
                int from = 2 * ((row * MAX_COLS) + col);
                int to = 2 * (((row - 1) * MAX_COLS) + col);
                video[to] = video[from];
                video[to + 1] = video[from + 1];
            }
        }

        for (int col = 0; col < MAX_COLS; col++) {
            int offset = 2 * ((MAX_ROWS - 1) * MAX_COLS + col);
            video[offset] = ' ';
            video[offset + 1] = 0x07;
        }

        cursor_row = MAX_ROWS - 1;
        cursor_col = 0;
    }
}


void print(const char *str) {
    while (*str) {
        print_char(*str++);
    }
}

void kernel_main() {
    clear_screen();
    while (1) {
        char c = keyboard_read_char();
        if (c) {
            if (c == '01') break; // ESC
            print((char[]){c, 0});
        }
    }
}