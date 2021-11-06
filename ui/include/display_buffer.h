#ifndef __DISPLAY_BUFFER_H__
#define __DISPLAY_BUFFER_H__

typedef struct _display_buffer_t
{
    char * buffer;
    int size;
}display_buffer_t;


int db_append(display_buffer_t * d_buffer, const char * data, int size);
void db_free(display_buffer_t * buf);

#endif // __DISPLAY_BUFFER_H__