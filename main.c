#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

struct termios orig_termios;

void error_handling(const char * error_msg)
{
    perror(error_msg);
    exit(1);
}

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

int entering_raw_mode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_iflag &= ~(IXON | ICRNL); // IXON disable ctrl-s and ctrl-q, ICRNL - ctrl-m
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cflag |= (CS8);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1; // This timing settings make nonblocking for read operation to wait only for 100 msec
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    return 0;
}


void clear_screen()
{
    write(STDOUT_FILENO, "\x1b[2J",4);// clear screen
    write(STDOUT_FILENO, "\x1b[H", 3);// move cursor to upper left
}

void draw_screen()
{
    int y;
    for(y = 0; y < 24; y++)
    {
        write(STDOUT_FILENO, "~\r\n",3);
    }
    write(STDOUT_FILENO, "\x1b[H", 3);// move cursor to upper left
}

char read_character()
{
    int rc_read;
    char c;
    while( (rc_read = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if(rc_read == -1 && errno != EAGAIN)
        {
            error_handling("read_character");
        }
    }
    return c;
}

void reading_operation()
{
    char c = read_character();
    switch (c) {
        case 'q':
            exit(0);
            break;
        default:
            write(STDOUT_FILENO,&c,1);
            break;
    }
}

int main()
{
    char c;
    entering_raw_mode();
    clear_screen();
    draw_screen();
    while (1) {
        reading_operation();
    }
    clear_screen();
}
