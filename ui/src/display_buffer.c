#include <string.h>
#include <stdlib.h>

#include "../include/display_buffer.h"

int db_append(display_buffer_t * d_buffer, const char * data, int size)
{
    int rc;

    rc = -1;

    char * new_buf = realloc((d_buffer->buffer), d_buffer->size + size);
    if( new_buf != NULL)
    {
        memcpy((void *)(&(new_buf[d_buffer->size])), data, size);
        d_buffer->buffer = new_buf;
        d_buffer->size += size;
        rc = 0;
    }

    return rc;
}

void db_free(display_buffer_t * buf)
{
    free((void * )(buf->buffer));
    buf->size = 0;
}