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
        //Draw first and last line
        if(y == 0 || y == (d_size.row_y - 1))
        {
            while(x--)
            {
                db_append(buf, "f", 1);
            }
        }
        //Draw welcome message
        else if(y == d_size.row_y / 4)
        {
            // Draw padding from left
            padding_left = (d_size.col_x - welcomelen) / 2;
            padding_right = padding_left;
            if(padding_left > 2)
            {
                db_append(buf, "f", 1);
                padding_left--;
            }
            while(padding_left--)
            {
                db_append(buf, " ", 1);
            }
            // Print welcome message
            db_append(buf, welcome, welcomelen);
            // Draw right padding
            while (padding_right)
            {
                padding_right--;
                db_append(buf, " ", 1);
            }
            db_append(buf, "f", 1);

        }
        // Draw login input field
        else if(y == (d_size.row_y / 3))
        {
            padding_left = (d_size.col_x - loginlen) / 2;
            padding_right = padding_left;
            if(padding_left > 2)
            {
                db_append(buf, "f", 1);
                padding_left--;
            }
            while(padding_left--)
            {
                db_append(buf, " ", 1);
            }
            db_append(buf, login_string, loginlen);
            // Draw right padding
            while (padding_right)
            {
                padding_right--;
                db_append(buf, " ", 1);
            }
            db_append(buf, "f", 1);
        }
        // Draw a frame
        else
        {
            db_append(buf, "f", 1);
            x -= 2;   // 1 glyph for left frame and one for right frame
            while(x--)
            {
                db_append(buf, " ", 1);
            }
            db_append(buf, "f", 1);
        }


        db_append(buf, "\x1b[K", 3);    // clear one line
        if (y < (d_size.row_y - 1)) 
        {
            db_append(buf, "\r\n", 2);
        }
    }
}