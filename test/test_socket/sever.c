#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define DEBUG
#include "../../src/debug.h"

FILE *file;
void close_html()
{
    fclose(file);
}   
#define PORT 8080
int main()
{
    char http_response[2048] = "HTTP/1.1 200 OK\r\n\r\n";
    // char server_message[512] = "<DOCTYPE! html><html><body><h1>Hello, World!</h1></body></html>\n";
    file = fopen("index.html", "r");
    atexit(close_html);
    char server_message[512];
    if (file == NULL)
    {
        debug_print("Error opening file\n");
        return -1;
    }
    while (fgets(server_message, sizeof(server_message), file) != NULL)
    {
        strncat(http_response, server_message, sizeof(server_message));
    }
    // strncat(http_response, server_message, sizeof(server_message));
    debug_print("The server sent the data: %s\n", http_response);
    // create the server socket
    int32_t server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET is the address family for IPv4, SOCK_STREAM is the socket type for TCP connection

    // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);       // htons() is used to convert the port number to network byte order  (big endian)
    server_address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY is the IP address of the machine the server is running on

    // bind the socket to our specified IP and port
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // listen for connections
    listen(server_socket, 5); // 5 is the number of connections that can be waiting while the server is handling a particular connection

    int32_t client_socket;
    while (1)
    {
        /* code */
   
    
    client_socket = accept(server_socket, NULL, NULL); // accept the connection

    // send the message
    send(client_socket, http_response, sizeof(http_response), 0);
    // debug_print("The server sent the data: %s\n", server_message);
    // and then close the socket
    close(server_socket);
    }
    return 0;
}