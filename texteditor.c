
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct termios TermIO;

static bool is_rawmode_enabled = false;
static TermIO original_config;
void rawmode_disable(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_config);
    is_rawmode_enabled = false;
}
void rawmode_enable(void) {
    if (is_rawmode_enabled) return;
    tcgetattr(STDIN_FILENO, &original_config);

    TermIO rawmode_config = original_config;

    rawmode_config.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    rawmode_config.c_oflag &= ~(OPOST);
    rawmode_config.c_cflag |= (CS8);
    rawmode_config.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    rawmode_config.c_cc[VMIN] = 0;
    rawmode_config.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawmode_config);
    is_rawmode_enabled = true;
}


int main(void) {
    rawmode_enable();
    atexit(rawmode_disable);

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');


    return 0;
}