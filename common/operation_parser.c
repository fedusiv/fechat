#include <stdlib.h>

#include "display.h"
#include "connect.h"
#include "input_capture.h"
#include "messaging_queue.h"
#include "common.h"

void application_exit();    // run exit application procedure
void parse_read_capture_messages(input_capture_message_t * msg);
void parse_internal_messages(internal_queue_s * msg);   // first step on parsing

void application_exit()
{
    disconnect_from_server();
    exit(0);
}


void parse_read_capture_messages(input_capture_message_t * msg)
{
    switch (msg->opcode)
    {
    case EXIT_READ_CODE:
        application_exit();
        break;
    
    default:
        break;
    }

    // need to free allocated data 
    free(msg);
}

void parse_internal_messages(internal_queue_s * msg)
{
    switch (msg->message.opcode)
    {
    case READ_CAPTURE_IN_OP:
        parse_read_capture_messages( (input_capture_message_t *)(msg->message.data) );
        break;
    
    default:
        break;
    }

    // need to free pointer of internal queue
    free(msg);
}


void * operation_parser_func(void * arg)
{
    while(1)
    {
        internal_queue_s * msg = get_next_internal_queue_message();
        if(msg == NULL)
        {
            continue;
        }
        parse_internal_messages(msg);
    }
}
