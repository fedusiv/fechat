#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <stdint.h>
#include <string.h>


#include "../include/display.h"
#include "../include/login_page.h"
#include "../../common/include/common.h"
#include "../include/display_buffer.h"
#include "../include/display_defines.h"


typedef struct _display_config_t 
{
    struct termios orig_termios;
    display_size_t d_size;  // size of display
    dispaly_cursor_t cursor;    // data about cursor
    display_buffer_t buffer;
}display_config_t;

static display_config_t display_config;

void init_display();            // init display function
void disable_raw_mode(void);    // disable raw mode of command line
int entering_raw_mode(void);    // enter raw mode of terminal
char read_character(void);      // read input from user
void reading_operation(void);   // parse income data from user
int get_window_size(display_size_t * d_size);  // get size of window, command line basically
int get_cursor_position(display_size_t * d_size);  // get current position of cursor
void draw_cursor(display_config_t * d_c);           // draw cursor position
void refresh_display();


void draw_cursor(display_config_t * d_c)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", d_c->cursor.cy + 1, d_c->cursor.cx + 1);  // +1 used, because in command line counts position from 1 index, not as in C from 0 index
    db_append(&(d_c->buffer), buf, strlen(buf));
}

int get_cursor_position(display_size_t * d_size)
{
    char buf[16];
    unsigned int i = 0;

    // "\x1b[6n" cursor position command
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    {
        return -1;
    }
    // Result of cursor position command is
    // ESC [ Pn ; Pn R
    while (i < sizeof(buf) - 1)
    {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) 
        {
            break; // incoming byte is written into buf. go to next loop iteration to parse output
        }
        if (buf[i] == 'R') 
        {
            // end of read buffer is 'R', 'R' is last symbol of position return command
            break;
        }
        i++;
    }
    buf[i] = '\0';  // to operate sscanf
    // verification, that we received exactly output of our required cursor position command
    if (buf[0] != '\x1b' || buf[1] != '[')
    {
        return -1;
    }
    // Trying to access of data about row and column
    if (sscanf(&buf[2], "%d;%d", &(d_size->row_y), &(d_size->col_x)) != 2)
    {
        return -1;
    }

    return 0;
}

int get_window_size(display_size_t * d_size)
{
    struct winsize ws;
    int rc;
    
    if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) 
    {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) 
        {
            rc = -1;    // failed to write moving cursor request
        }
        else
        {
            rc = get_cursor_position(d_size);   // call position request
        }
    }
    else
    {
        d_size->col_x = ws.ws_col;
        d_size->row_y = ws.ws_row;
        rc = 0;
    }

    return rc;
}

void disable_raw_mode()
{
    clear_screen();
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &display_config.orig_termios) == -1)
    {
        error_handling("disable raw mode, tcsetattr");
    }
    printf("\033[H\033[J"); // Clear screen completely
}

int entering_raw_mode()
{
    struct termios raw;
    
    if(tcgetattr(STDIN_FILENO, &display_config.orig_termios) == -1)
    {
        error_handling("enter raw mode, tcsetattr");
    }
    atexit(disable_raw_mode);   // set run this function on exit of app

    raw = display_config.orig_termios;
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

void refresh_display()
{
    display_buffer_t * d_b = &(display_config.buffer);

    db_append(d_b, "\x1b[?25l", 6);  // hide cursor
    db_append(d_b, "\x1b[H", 3); // move cursor to upper right corner

    draw_login_page(d_b, display_config.d_size);
    draw_cursor(&(display_config));
    
    db_append(&display_config.buffer, "\x1b[?25l", 6);  // hide cursor
    db_append(&display_config.buffer, "\x1b[H", 3);

    write(STDOUT_FILENO, d_b->buffer, d_b->size);
    db_free(d_b);
}


void init_display()
{
    int rc;

    entering_raw_mode();
    clear_screen();

    display_config.cursor.cx = 0;
    display_config.cursor.cy = 0;

    rc = get_window_size(&(display_config.d_size));
    if(rc != 0)
    {
        error_handling("Error to init display");
    }

}

void * ui_thread_func(void * arg)
{
    char c;
    init_display();
    while (1) 
    {
        refresh_display();
        reading_operation();
    }
    clear_screen();
}
