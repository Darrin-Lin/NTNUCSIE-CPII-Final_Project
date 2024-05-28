#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#define DEBUG
#include "../../src/debug.h"

#define PORT 8080
#define MAXLINE 1024


int main(int argc, char const *argv[])
{
    // create socket
    int32_t net_socket;
    net_socket  = socket(AF_INET, SOCK_STREAM, 0); // AF_INET is the address family for IPv4, SOCK_STREAM is the socket type for TCP connection

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT); // htons() is used to convert the port number to network byte order  (big endian)
    server_address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY is the IP address of the machine the server is running on

    int32_t connect_status = connect(net_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connect_status == -1)
    {
        debug_print("There was an error making a connection to the remote socket\n");
        return -1;
    }

    // receive data from the server
    char server_response[MAXLINE];
    recv(net_socket, &server_response, sizeof(server_response), 0);

    // print out the server's response
    debug_print("The server sent the data: %s\n", server_response);
    
    // and then close the socket
    close(net_socket);
    return 0;
}