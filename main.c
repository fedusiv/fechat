#include <pthread.h>
#include "ui/include/display.h"

int main()
{
    pthread_t ui_thread;

    pthread_create(&ui_thread, NULL, ui_thread_func, NULL); // create ui thread
    pthread_join(ui_thread, NULL);

    return 0;
}
