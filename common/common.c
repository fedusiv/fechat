#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

APP_STATE g_application_state;  // global state of appliaction

APP_STATE get_application_state()
{
    return g_application_state;
}

void clear_screen()
{
    write(STDOUT_FILENO, "\x1b[2J",4);// clear screen
    write(STDOUT_FILENO, "\x1b[H", 3);// move cursor to upper left
}

void error_handling(const char * error_msg)
{
    clear_screen();
    printf("Program finished with error\n");
    perror(error_msg);
    exit(1);
}
