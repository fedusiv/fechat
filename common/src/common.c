#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"

APP_STATE g_application_state;  // global state of appliaction

APP_STATE get_application_state()
{
    return g_application_state;
}

void error_handling(const char * error_msg)
{
    perror(error_msg);
    exit(1);
}
