#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f) // 0x1f = 00011111

/*** data ***/

struct termios original_termios;


/*** terminal ***/

void die(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

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

char editorReadKey() {
    int nread;
    char c;
    while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if(nread == -1 && errno != EAGAIN) die("read");
    }
    return c;
}


/*** output ***/

void editorDrawRows() {
    int y;
    for(y = 0; y < 24; y++) {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}

void editorRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4); // 2J: clear the entire screen
    write(STDOUT_FILENO, "\x1b[H", 3); // H: cursor position

    editorDrawRows();
    write(STDOUT_FILENO, "\x1b[H", 3);
}


/*** input ***/

void editorProcessKeypress() {
    char c = editorReadKey();
    switch(c) {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
    }
}



int main() {
    enableRawMode();
    while(1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }
    return 0;
}