#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f) // 0x1f = 00011111

struct termios original_termios;



void die(const char *s) {
    perror(s);
    exit(1);
}

void disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if(tcgetattr(STDIN_FILENO, &original_termios) == -1) 
        die("tcgetattr");
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

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) 
        die("tcsetattr");
}

int main() {
    enableRawMode();
    while(1) {

    char c = '\0';
    
        if (read(STDIN_FILENO, &c, 1)== -1 && errno!= EAGAIN) die("read");
            if (iscntrl(c)) printf("%d\r\n", c);
            else printf("%d ('%c')\r\n", c, c);
            if (c == CTRL_KEY('q')) break;
        }
    return 0;
}