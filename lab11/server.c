#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <unistd.h> 

#include "protocol_specs.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }

    uint32_t ip_address = inet_addr(argv[1]);
    uint16_t port = (uint16_t)strtol(argv[2], NULL, 10);

    struct sockaddr_in addr = {
        .sin_addr.s_addr = ip_address,
        .sin_port = htons(port),
        .sin_family = AF_INET
    };
    
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    int t = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int));
    if(bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        perror("bind"); 

    if(listen(socket_fd, MAX_CLIENTS) < 0)
        perror("listen");

    int clients_fd_array[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++)
        clients_fd_array[i] = -1;


    pid_t acceptloop_pid = fork();
    if (acceptloop_pid < 0)
        perror("fork");
    else if (acceptloop_pid == 0) {
        while (1) {
            int client_fd;
            if((client_fd = accept(socket_fd, NULL, 0)) < 0)
                perror("accept");

            int i = 0;
            while (i < MAX_CLIENTS){
                if (clients_fd_array[i] == -1) {
                    clients_fd_array[i] = client_fd;
                    printf("Client accepted\n");
                    printf("Client_fd: %d %d \n", i, client_fd);
                    break;
                }

                i++;
            }

            if (i == MAX_CLIENTS)
                printf("Client limit reached\n");
        }  
    } else {
         while (1) {
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients_fd_array[i] == -1)
                    continue;

                printf("trying\n");

                request_message_t message;
                int a = recv(clients_fd_array[i], &message, sizeof(message), MSG_DONTWAIT);
                if(a > 0) {
                    printf("received");
                    switch(message.request_type) {
                        case TOALL:
                            printf("TO_ALL: %s FROM: %s\n", message.payload.to_all, message.sender_client_id);
                            break;
                        
                        case TOONE:
                            printf("TO_ONE: %s %s FROM: %s \n", message.payload.to_one.target_client_id, message.payload.to_one.message, message.sender_client_id);
                            break;

                        case LIST:
                            printf("LIST FROM: %s", message.sender_client_id);
                            break;
                    }
                }
            }
        }
    }
   
    return 0;
}