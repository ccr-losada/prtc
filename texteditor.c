
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include "types.h"

#define KEYMASK_CTRL 0x1f

typedef struct termios TermIO;

static bool is_rawmode_enabled = false;
static TermIO original_config;
void rawmode_disable(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_config);
    is_rawmode_enabled = false;
}
void rawmode_enable(void) {
    if (is_rawmode_enabled == true) return;
    tcgetattr(STDIN_FILENO, &original_config);

    TermIO rawmode_config = original_config;

    rawmode_config.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    rawmode_config.c_oflag &= ~(OPOST);
    rawmode_config.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    rawmode_config.c_cflag |= (CS8);
    rawmode_config.c_cc[VMIN] = 0;
    rawmode_config.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawmode_config) == -1) {
        fprintf(stderr, "tcsetattr");
        exit(1);
    }
    is_rawmode_enabled = true;
}

void clear() {
    write(STDOUT_FILENO, "\x1b[2J", 4); // Clear screen.
    write(STDOUT_FILENO, "\x1b[H", 3);  // Reposition cursor.
}

int utf8_size(UInt8 byte) {
    if ((byte & 0x80) == 0x00) return 1; 
    if ((byte & 0xE0) == 0xC0) return 2;
    if ((byte & 0xF0) == 0xE0) return 3;
    if ((byte & 0xF8) == 0xF0) return 4;

    if ((byte & 0xC0) == 0x80) return 0; // Continuation byte.
    return -1; // Invalid UTF-8 byte.
}

void display_input(UInt8 c) {
    printf("%u (%03u)", utf8_size(c), c);
    if (isprint(c)) printf(" %c", c);
    printf("\r\n");
}

UInt16 cursor = 0;
constexpr UInt16 buffer_size = 128;
UInt8 buffer[buffer_size] = {0};

void process_input(UInt8 c) {
    //display_input(c);
    //if (utf8_size(c) == 1 && isprint(c)) write(STDOUT_FILENO, &c, 1);
    //if (c == 13) write(STDOUT_FILENO, "\r\n", 2);

    buffer[cursor] = c;
    if (cursor < buffer_size) cursor++;


    if (c == (KEYMASK_CTRL & 'q')) exit(0);
}

int main(void) {
    rawmode_enable();
    atexit(rawmode_disable);

    clear();
    atexit(clear);

    char c;
    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
            fprintf(stderr, "read");
            exit(1);
        }

        if (c != '\0') process_input(c);
        clear();
        write(STDOUT_FILENO, buffer, cursor);
        

    }
    return 0;
}