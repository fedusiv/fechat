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


#include "display.h"
#include "login_page.h"
#include "common.h"
#include "display_buffer.h"
#include "display_defines.h"

typedef struct _display_config_t 
{
    struct termios orig_termios;
    display_size_t d_size;  // size of display
    dispaly_cursor_t cursor;    // data about cursor
    display_buffer_t * buffer;
}display_config_t;

display_config_t display_config;

void init_display();            // init display function
void disable_raw_mode(void);    // disable raw mode of command line
int entering_raw_mode(void);    // enter raw mode of terminal
char read_character(void);      // read input from user
void reading_operation(void);   // parse income data from user
int get_window_size(display_size_t * d_size);  // get size of window, command line basically
int get_cursor_position(display_size_t * d_size);  // get current position of cursor
void draw_cursor(display_config_t * d_c);           // draw cursor position
void refresh_display();
void draw_login_page(display_config_t * conf, display_buffer_t * buf ,display_size_t d_size);


void draw_login_page(display_config_t * conf, display_buffer_t * buf ,display_size_t d_size)
{
    int y, x;
    int welcomelen, loginlen; // real amount of bytes, which should be inserted into one welcome line
    const char welcome_string[] = "Welcome to fe(dusiv)chat";
    const char login_string[] = "login: ";
    const char password_string[] = "password: ";
    char welcome[80];   // string to store welcome string
    int padding_left, padding_right;    // amount of space from one side to welcome message

    loginlen = sizeof(login_string);

    // prepare welcome message
    welcomelen = snprintf(welcome, sizeof(welcome),"%s ver:%s", welcome_string ,APP_VERSION);
    if (welcomelen > d_size.col_x)
    {
        welcomelen = d_size.col_x;
    }

    for(y = 0; y < d_size.row_y; y++)
    {
        x = d_size.col_x;
        //Draw welcome message
        if(y == d_size.row_y / 4)
        {
            // Draw padding from left
            padding_left = (d_size.col_x - welcomelen) / 2;
            padding_right = padding_left;
            while(padding_left--)
            {
                db_append(buf, " ", 1);
            }
            // Print welcome message
            db_append(buf, welcome, welcomelen);
        }
        // Draw login input field
        else if(y == (d_size.row_y / 3))
        {
            padding_left = (d_size.col_x - loginlen) / 2;
            conf->cursor.cx = padding_left + loginlen;
            padding_right = padding_left;
            while(padding_left--)
            {
                db_append(buf, " ", 1);
            }
            // At this position need to store cursor position
            conf->cursor.cy = y;
            db_append(buf, login_string, loginlen);
        }
        // Draw a frame
        else
        {

        }


        db_append(buf, "\x1b[K", 3);    // clear one line
        if (y < (d_size.row_y - 1)) 
        {
            db_append(buf, "\r\n", 2);
        }
    }
}

void draw_cursor(display_config_t * d_c)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", d_c->cursor.cy + 1, d_c->cursor.cx + 1);  // +1 used, because in command line counts position from 1 index, not as in C from 0 index
    db_append(d_c->buffer, buf, strlen(buf));
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
    system("tput rmcup"); // exit alternate screen
}

int entering_raw_mode()
{
    struct termios raw;

    system("tput smcup");   // enter alternate screen

    if(tcgetattr(STDIN_FILENO, &display_config.orig_termios) == -1)
    {
        error_handling("enter raw mode, tcsetattr");
    }
    atexit(disable_raw_mode);   // set run this function on exit of app

    raw = display_config.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_iflag &= ~(IXON | ICRNL); // IXON disable ctrl-s and ctrl-q, ICRNL - ctrl-m
    raw.c_oflag &= ~(OPOST); // playing with carriage return, to get more control
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); // turn off canonical mode, sigint(ctrl-z, ctrl-c), disable ctrl-v
    raw.c_cflag |= (CS8);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1; // This timing settings make nonblocking for read operation to wait only for 100 msec
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);


    return 0;
}

void refresh_display()
{
    // Init buffer to display
    display_buffer_t buf;
    buf.size = 0;
    buf.buffer = NULL;
    display_config.buffer = &buf;

    db_append(&buf, "\x1b[?25l", 6);  // hide cursor
    db_append(&buf, "\x1b[H", 3); // move cursor to upper right corner

    draw_login_page(&display_config, &buf, display_config.d_size);
    draw_cursor(&(display_config));
    
    db_append(&buf, "\x1b[?25h", 6); // show cursor

    write(STDOUT_FILENO, buf.buffer, buf.size);
    db_free(&buf);
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
