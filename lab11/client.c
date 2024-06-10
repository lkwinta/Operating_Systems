#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <unistd.h> 

#include "protocol_specs.h"

volatile bool should_close = false;

void sigint_handler(int signo) {   
    should_close = true;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <ip> <port> <self identifier> \n", argv[0]);
        return -1;
    }

    signal(SIGINT, sigint_handler);

    char* client_identifier = argv[1];
    uint32_t ip_address = inet_addr(argv[2]);
    uint16_t port = (uint16_t)strtol(argv[3], NULL, 10);

    struct sockaddr_in addr = {
        .sin_addr.s_addr = ip_address,
        .sin_port = htons(port),
        .sin_family = AF_INET
    };
    
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(socket_fd, &addr, sizeof(addr)) < 0)
        perror("connect");

    pid_t listener_pid = fork();
    if (listener_pid < 0)
        perror("fork listener");
    else if (listener_pid == 0) {
        while (!should_close) {
            request_message_t message;
            recv(socket_fd, &message, sizeof(message), MSG_WAITALL);
            switch(message.request_type) {

            }
        }
    } else {
        char* request_type_input_buffer = NULL;
        while (!should_close) {
            if(scanf("%ms", &request_type_input_buffer) == 1) {
                request_message_t message;
                memcpy(message.sender_client_id, client_identifier, strlen(client_identifier));

                if (strcmp(request_type_input_buffer, "LIST") == 0){
                    message.request_type = LIST;

                    if(send(socket_fd, &message, sizeof(message), MSG_DONTWAIT) < 0)
                        printf("err sendd");
                } else if (strcmp(request_type_input_buffer, "2ALL") == 0) {
                    message.request_type = TOALL;
                    scanf("%s", &message.payload.to_all);
                    send(socket_fd, &message, sizeof(message), MSG_DONTWAIT);
                } else if (strcmp(request_type_input_buffer, "2ONE") == 0) {
                    message.request_type = TOONE;
                    scanf("%s", &message.payload.to_one.target_client_id);
                    scanf("%s", &message.payload.to_one.message);
                    send(socket_fd, &message, sizeof(message), MSG_DONTWAIT);
                } else {
                    printf("Invalid request type! \n");
                }

                free(request_type_input_buffer);
            } else 
                perror("scanf input");
            
        }
    }
  
    return 0;
}