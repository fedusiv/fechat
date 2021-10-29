#ifndef __INPUT_CAPTURE_H__
#define __INPUT_CAPTURE_H__

typedef enum INPUT_CAPTURE_MESSAGE_OPCODE_E
{
    EXIT_READ_CODE
}INPUT_CAPTURE_MESSAGE_OPCODE;

typedef struct INPUT_CAPTURE_MESSAGE_S
{
    INPUT_CAPTURE_MESSAGE_OPCODE opcode;
}input_capture_message_t;


void * read_thread_func(void * arg);

#endif
