#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "../include/connection_defines.h"
#include "../include/connect.h"
#include "../../common/include/common.h"


// variables declaration
static int socket_fd;  // connection socket
static struct sockaddr_in server_addr;
static unsigned char buff[RECEIVE_BUFFER_SIZE];


// functions declaration
void fill_server_info();
void socket_connection_handler();
int connect_to_server();
void connect_error_handling(const char * error_msg);


/*
*   Connection error handler, because we also need to close socket is something happend
*/
void connect_error_handling(const char * error_msg)
{
    if(socket_fd != -1)
    {
        close(socket_fd);
    }
    error_handling(error_msg);
}

void fill_server_info()
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);
}

/*
*   loop of connection thread
*/
void socket_connection_handler()
{
    int rc;

    // Set socket to be in non blocking mode
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = RECEIVE_TIMEOUT_MS * 1000; // 1000 because tv in microsec
    setsockopt(socket_fd, SOL_SOCKET,SO_RCVTIMEO,&tv, sizeof(tv));

    while (1)
    {
        rc = recv(socket_fd, buff, RECEIVE_BUFFER_SIZE, 0);
        if( rc > 0) // got message
        {
            write(STDIN_FILENO, "yep\r\n", 5);
        }
        else if ( rc == 0 || rc == EAGAIN || rc == EWOULDBLOCK) // received nothing
        {

        }
        else if( rc == -1)  // some error occured
        {
            //TODO: issue, socket fails here if no info came!
        }
        else
        {
            connect_error_handling("Undetermined issue in recv");
        }

    }
    
}

/*
*   connect to server function
*/
int connect_to_server()
{
    int rc;
    int connection_tries = 0;

    while(rc != 0)
    {
        // connection loop
        rc = connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        sleep(SERVER_CONNECTION_TIMEOUT_TRY_SEC);
        connection_tries++;

        if(connection_tries >= SERVER_CONNECTION_TRIES)
        {
            // big amount of tries, exit of connection
            break;
        }
    }

    return rc;
}


/*
*   main function for connection, which runs on thread
*/
void * connect_thread_func(void * arg)
{
    int rc;

    fill_server_info();

    // create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1)
    {
        connect_error_handling("Failed to create socket");
    }

    rc = connect_to_server();
    if(rc != 0)
    {
        connect_error_handling("Can not connect to server");
    }

    socket_connection_handler();    // loop of connection

    close(socket_fd);
}
