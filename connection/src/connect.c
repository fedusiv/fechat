#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/connection_defines.h"
#include "../include/connect.h"
#include "../../common/include/common.h"


// variables declaration
static int socket_fd;  // connection socket
static struct sockaddr_in server_addr;


// functions declaration
void fill_server_info();
void socket_connection_handler();


void fill_server_info()
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);
}


void socket_connection_handler()
{
    while (1)
    {
        /* code */
    }
    
}

/*
*   main function for connection, which runs on thread
*/
void * connect_thread_func(void * arg)
{
    fill_server_info();

    // create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1)
    {
        error_handling("Failed to create socket");
    }

    socket_connection_handler();    // loop of connection
}