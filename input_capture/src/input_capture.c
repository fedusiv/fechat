#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "../../common/include/common.h"
#include "../include/input_capture.h"
#include "../../common/include/messaging_queue.h"


void create_internal_read_message(INPUT_CAPTURE_MESSAGE_OPCODE opcode)
{
    input_capture_message_t * msg = malloc(sizeof(input_capture_message_t));
    msg->opcode = opcode;
    create_internal_message(READ_CAPTURE_IN_OP, (void *) msg);
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
            create_internal_read_message(EXIT_READ_CODE);
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