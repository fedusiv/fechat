/*
* Module responsible for queue of messages between threads.
* Queue implemented in linked list
*/
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include "messaging_queue.h"

internal_queue_s internal_message_head;
pthread_mutex_t queue_lock;


int init_internal_queue_lock()
{
    int rc = 0;
    if (pthread_mutex_init(&queue_lock, NULL) != 0)
    {
        rc = -1;
    }

    return rc;
}

/*
*   Create internal message queue element
*/
int create_internal_message(INTERNAL_OPCODE opcode, void * data)
{
    pthread_mutex_lock(&queue_lock);    // infinite blocking

    internal_queue_s * next_element = (internal_queue_s *)internal_message_head.next;
    while(next_element != NULL)
    {
        next_element = (internal_queue_s *)next_element->next;
    }

    next_element = (internal_queue_s *) malloc(sizeof(internal_queue_s));
    next_element->message.opcode = opcode;
    next_element->message.data = data;
    next_element->next = NULL;

    internal_message_head.next = (struct internal_queue_s *)next_element;

    pthread_mutex_unlock(&queue_lock);

    return 0;
}

/*
*   return first internal queue message
*   Do not forget to free memory region on this pointer
*/
internal_queue_s * get_next_internal_queue_message()
{
    pthread_mutex_lock(&queue_lock);    // infinite blocking

    internal_queue_s * element = (internal_queue_s *)internal_message_head.next;
    if(element != NULL)
    {
        internal_message_head.next = element->next;
    }

    pthread_mutex_unlock(&queue_lock);

    return element;
}
