// Headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Defining the main function.
int main(int argc, char *argv[]) {
    // Declaring structs
    struct sockaddr_in server_addr, client_addr;
    socklen_t cleint_addr_len;

    // Creating a socket using the socket() function.
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);        /* First argument specifies IPv4, second is specifying stream socket, and third is default protocol TCP/IP. */
    // Checking if creating the socket failed
    if(server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Setting all bytes in structure to 0
    memset(&server_addr, '0', sizeof(server_addr));
    // sin_family field specifies IPv4
    server_addr.sin_family = AF_INET;
    // sin_addr.s_addr field specifies IP address. INADDR_ANY binds to all available network interfaces.
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    // sin_port field specifies the port number to listen on
    server_addr.sin_port = htons(8080);

    /*  Bind the socket to the server address using the bind() function.
    *   First argument is the socket file descriptor, second is a pointer to the server address, third is the size of the server address structure.
    *   Error checking is also included to see if the bind() function has failed.
    */
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed.");
        exit(EXIT_FAILURE);
    }

    /*  Listening for incoming connections using the listen() function.
    *   First argument is the socket file descriptor, second is the max number of pending connections that can be queued before they are accepted.
    *   Error checking is included.
    */
    if(listen(server_fd, 3) < 0) {
        perror("Listen failed.");
        exit(EXIT_FAILURE);
    }



    /*  Accepts incoming connections using the accept() function.
    *   First argument is the socket file descriptor, second is a pointer to a client address structure that will be filled by the accept() function.
    *   Third argument is a pointer to the size of the client address structure.
    *   Error checking included as well.
    */
    // Defining the client_addr_len variable and setting its value to  the size of the client_addr structure.
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&client_addr_len);
    if(client_fd < 0) {
        perror("Accept failed.");
        exit(EXIT_FAILURE);
    }

    /*  Receives data from the client using the recv() function.
    *   The first argument is the client socket file descriptor, the second is a pointer to a buffer that will hold the received data.
    *   The third argument is the max size of the buffer, and the fourth is a set of flags that control the behavior of the function.
    */
    char buffer[1024] = {0};
    int valread = recv(client_fd, buffer, 1024, 0);
    /*  After receiving data from the client, we send the same data back to the client using the send() function.
    *   The first argument is the client socket file descriptor, the second is a pointer to the data to send, the third is the size of the data to send.
    *   The fourth argument is a set of flags that control the behavior of the function.
    */
    send(client_fd, buffer, strlen(buffer), 0);

    // Close the client socket
    close(client_fd);
    // Close server socket
    close(server_fd);

    return 0;
}

