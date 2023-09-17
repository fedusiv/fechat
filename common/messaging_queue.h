#ifndef __MESSAGING_QUEUE_H__
#define __MESSAGING_QUEUE_H__

typedef enum INTERNAL_OPCODE_E
{
    READ_CAPTURE_IN_OP,
    COMM_MESSAGE_IN_OP
}INTERNAL_OPCODE;

typedef struct INTERNAL_MESSAGE_S
{
    INTERNAL_OPCODE opcode;
    void * data;
}internal_message_s;

typedef struct INTERNAL_QUEUE_S
{
    internal_message_s message;
    struct internal_queue_s * next;
}internal_queue_s;

int create_internal_message(INTERNAL_OPCODE opcode, void * data);   // create next message in internal list for prsing
internal_queue_s * get_next_internal_queue_message();   // get pointer to first message in list of internal messages
int init_internal_queue_lock(); // initialization of mutex, related to internal messages operation
#endif // __MESSAGING_QUEUE_H__
