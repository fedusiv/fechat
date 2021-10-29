#include <unistd.h>
#include <errno.h>
#include "../../common/include/common.h"


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
            error_handling("exit");
            break;
        default:
            break;
    }
}

void * read_thread_func(void * arg)
{
    while(1)
    {
        reading_operation();
    }
}