#include <stdint.h>
#include "screen.h"

#define VIDEO_ADDRESS 0xB8000
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x07

static uint16_t* video_memory = (uint16_t*) VIDEO_ADDRESS;
static int cursor_row = 0;
static int cursor_col = 0;

#define MAX_ROWS 25

static void scroll_if_needed() {
    if (cursor_row >= MAX_ROWS) {
        for (int row = 1; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLS; col++) {
                video_memory[(row - 1) * MAX_COLS + col] = video_memory[row * MAX_COLS + col];
            }
        }
        for (int col = 0; col < MAX_COLS; col++) {
            video_memory[(MAX_ROWS - 1) * MAX_COLS + col] = (WHITE_ON_BLACK << 8) | ' ';
        }
        cursor_row = MAX_ROWS - 1;
    }
}

static void print_char(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        video_memory[cursor_row * MAX_COLS + cursor_col] = (WHITE_ON_BLACK << 8) | c;
        cursor_col++;
        if (cursor_col >= MAX_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    scroll_if_needed();
}


void print_string(const char* str) {
    while (*str) {
        print_char(*str++);
    }
}

void clear_screen() {
    for (int i = 0; i < MAX_COLS * 25; i++) {
        video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
    }
    cursor_row = 0;
    cursor_col = 0;
}
