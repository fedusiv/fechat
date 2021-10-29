#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>


#include "../include/display.h"
#include "../include/login_page.h"
#include "../../common/include/common.h"

static struct termios orig_termios;

void disable_raw_mode(void);    // disable raw mode of command line
int entering_raw_mode(void);    // enter raw mode of terminal
void clear_screen(void);        // clear screen and move cursor and the left top bottom
char read_character(void);      // read input from user
void reading_operation(void);   // parse income data from user

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

int entering_raw_mode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);   // set run this function on exit of app

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

void display_operations()
{

}

void * ui_thread_func(void * arg)
{
    char c;
    entering_raw_mode();
    clear_screen();
    draw_login_page();
    while (1) {
        display_operations();
    }
    clear_screen();
}
