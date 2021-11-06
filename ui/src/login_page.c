#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

#include "../include/login_page.h"
#include "../../common/include/common.h"


void draw_login_page(display_buffer_t * buf, display_size_t d_size)
{
    int y;
    int welcomelen; // real amount of bytes, which should be inserted into one welcome line
    const char welcome_string[] = "Welcome to fe(dusiv)chat";
    char welcome[80];   // string to store welcome string
    int padding;    // amount of space from one side to welcome message

    // prepare welcome message
    welcomelen = snprintf(welcome, sizeof(welcome),"%s ver:%s", welcome_string ,APP_VERSION);
    if (welcomelen > d_size.col_x)
    {
        welcomelen = d_size.col_x;
    }


    for(y = 0; y < d_size.row_y; y++)
    {
        if(y == d_size.row_y / 3)
        {
            padding = (d_size.col_x - welcomelen) / 2;
            if(padding > 2)
            {
                db_append(buf, "f", 1);
                padding--;
            }
            while(padding--)
            {
                db_append(buf, " ", 1);
            }

            db_append(buf, welcome, welcomelen);
        }
        else
        {
            db_append(buf, "f", 1);
        }


        db_append(buf, "\x1b[K", 3);    // clear one line
        if (y < (d_size.row_y - 1)) 
        {
            db_append(buf, "\r\n", 2);
        }
    }
}