#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"


void error_handling(const char * error_msg)
{
    perror(error_msg);
    exit(1);
}
