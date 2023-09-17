#include <pthread.h>
#include <stdlib.h>
#include "display.h"
#include "connect.h"
#include "messaging_queue.h"
#include "operation_parser.h"

int main()
{
    pthread_t ui_thread, connect_thread, 
                input_thread, operation_thread;

    init_internal_queue_lock();

    pthread_create(&ui_thread, NULL, ui_thread_func, NULL); // create ui thread
    pthread_create(&connect_thread, NULL, connect_thread_func, NULL);  // create socket connection thread
    pthread_create(&operation_thread, NULL, operation_parser_func, NULL);  // create thread for user input


    pthread_join(ui_thread, NULL);
    pthread_join(connect_thread, NULL);
    pthread_join(input_thread, NULL);


    return 0;
}
