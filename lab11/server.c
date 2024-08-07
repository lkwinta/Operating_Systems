#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>

#include "protocol_specs.h"

volatile bool should_close = false;

// signal handler for SIGINT to close the server
void sigint_handler(int signo) {   
    should_close = true;
}

int main(int argc, char** argv) {
    // check for correct usage
    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }

    // register signal handler for SIGINT
    signal(SIGINT, sigint_handler);

    // parse ip and port
    uint32_t ip_address = inet_addr(argv[1]);
    uint16_t port = (uint16_t)strtol(argv[2], NULL, 10);

    // create socket with ip and port as tcp ip socket
    struct sockaddr_in addr = {
        .sin_addr.s_addr = ip_address,
        .sin_port = htons(port),
        .sin_family = AF_INET
    };
    
    // create socket 
    int socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    int t = 1;
    // set socket options
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int));
    // bind socket to address
    if(bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        perror("bind"); 

    // listen for connections
    if(listen(socket_fd, MAX_CLIENTS) < 0)
        perror("listen");

    // array to store client fds
    int clients_fd_array[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++)
        clients_fd_array[i] = -1;

    // array to store client ids
    bool clients_id_set[MAX_CLIENTS] = {0};
    char clients_id_array[MAX_CLIENTS][MAX_CLIENT_ID_LEN] = {0};
    clock_t clients_alive_timeout[MAX_CLIENTS];

    clock_t ping_time = clock();

    // main listen loop
    while (!should_close) {
        int client_fd;
        // accept new client
        if((client_fd = accept(socket_fd, NULL, 0)) > 0) {
            int i = 0;
            // find first empty slot in clients_fd_array
            while (i < MAX_CLIENTS){
                if (clients_fd_array[i] == -1) {
                    clients_fd_array[i] = client_fd;
                    clients_alive_timeout[i] = clock();

                    printf("Client accepted at index: %d\n", i);
                    break;
                }

                i++;
            }
             
            // close connection if client limit reached
            if (i == MAX_CLIENTS) {
                printf("Client limit reached\n");
                close(client_fd);
            }
        }

        // check for messages from clients
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients_fd_array[i] == -1)
                continue;

            // read message from client
            request_message_t message;
            if(recv(clients_fd_array[i], &message, sizeof(message), MSG_DONTWAIT) > 0) {
                // handle message based on request type
                switch(message.request_type) {
                    case TOALL:
                        // send message to all clients except sender
                        printf("TO_ALL: %s FROM: %s \n", message.payload.to_all, message.sender_client_id);
                        for (int j = 0; j < MAX_CLIENTS; j++)
                            if (clients_fd_array[j] != -1 && i != j)
                                send(clients_fd_array[j], &message, sizeof(message), MSG_DONTWAIT);
                        break;
                    
                    case TOONE:
                        // send message to specific client
                        printf("TO_ONE: %s %s FROM: %s \n", message.payload.to_one.target_client_id, message.payload.to_one.message, message.sender_client_id);
                        // find client with target_client_id and send message
                        for (int i = 0; i < MAX_CLIENTS; i++)
                            if (clients_fd_array[i] != -1 && strncmp(clients_id_array[i], message.payload.to_one.target_client_id, MAX_CLIENT_ID_LEN) == 0)
                                send(clients_fd_array[i], &message, sizeof(message), MSG_DONTWAIT);
                        break;

                    case LIST:
                        // send list of all clients
                        printf("LIST FROM: %s\n", message.sender_client_id);
                        int length = 0;
                        for (int j = 0; j < MAX_CLIENTS; j++)
                            if (clients_fd_array[j] != -1){
                                length++;
                                strncpy(message.payload.list.identifiers_list[j], clients_id_array[j], MAX_CLIENT_ID_LEN);
                            }
                        message.payload.list.list_length = length;

                        send(clients_fd_array[i], &message, sizeof(message), MSG_DONTWAIT);
                        break;

                    case ALIVE:
                        // update client alive timeout
                        printf("ALIVE FROM: %s\n", message.sender_client_id);
                        clients_alive_timeout[i] = clock();
                        if (!clients_id_set[i])
                            strncpy(clients_id_array[i], message.sender_client_id, MAX_CLIENT_ID_LEN);
                        break;

                    case STOP:
                        // close client connection
                        printf("STOP FROM: %s\n", message.sender_client_id);
                        clients_fd_array[i] = -1;
                        clients_id_set[i] = false;
                        break;
                }

                // flush stdout
                fflush(stdout);
            }
        }

        // send ALIVE messages to all clients every second
        if ((clock() - ping_time) / CLOCKS_PER_SEC > 1) {
            request_message_t alive_message = {
                .request_type = ALIVE
            };
            // send alive message to all clients
            for (int i = 0; i < MAX_CLIENTS; i++)
                if (clients_fd_array[i] != -1)
                    send(clients_fd_array[i], &alive_message, sizeof(alive_message), MSG_DONTWAIT);
            ping_time = clock();
        }

        // check for client timeouts
        for (int i = 0; i < MAX_CLIENTS; i++)
            // close client connection if timeout
            if (clients_fd_array[i] != -1 && (clock() - clients_alive_timeout[i]) / CLOCKS_PER_SEC > 5) {
                printf("Client %s timed out\n", clients_id_array[i]);
                close(clients_fd_array[i]);
                clients_fd_array[i] = -1;
                clients_id_set[i] = false;
            }
    }

// close all client connections
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (clients_fd_array[i] != -1)
            close(clients_fd_array[i]);
   
    return 0;
}
