#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios original_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &original_termios);
    atexit(disableRawMode);

    struct termios raw = original_termios;
    // c_iflag: input flag
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); //IXON: Ct-s Ct-q ICRNL: Ct-m
    // c_oflag: output flag
    raw.c_oflag &= ~(OPOST);
    // c_cflag: control flag
    raw.c_cflag |= (CS8);
    // c_lflag: local flag
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); //ISIG: Ct-c Ct-z IEXTEN: Ct-v ICANON: canonical mode 

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    while(read(STDIN_FILENO, &c, 1)==1 && c!= 'q') {
        if (iscntrl(c)) printf("%d\r\n", c);
        else printf("%d ('%c')\r\n", c, c);
        if (c == 'q') break;
    }
    
    return 0;
}