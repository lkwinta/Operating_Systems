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
#include <string.h>

#include "protocol_specs.h"

volatile bool should_close = false;

// signal handler for SIGINT to close the client 
void sigint_handler(int signo) {   
    should_close = true;
}

int main(int argc, char** argv) {
    // check for correct usage
    if (argc < 3) {
        printf("Usage: %s <ip> <port> <self identifier> \n", argv[0]);
        return -1;
    }

    // register signal handler for SIGINT
    signal(SIGINT, sigint_handler);

    // parse ip and port
    char* client_identifier = argv[1];
    uint32_t ip_address = inet_addr(argv[2]);
    uint16_t port = (uint16_t)strtol(argv[3], NULL, 10);

    // create socket with ip and port as tcp ip socket
    struct sockaddr_in addr = {
        .sin_addr.s_addr = ip_address,
        .sin_port = htons(port),
        .sin_family = AF_INET
    };

    // create socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // connect to server
    if(connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        perror("connect");

    // send alive message to notify server of client
    request_message_t alive_message = {
        .request_type = ALIVE
    };
    strncpy(alive_message.sender_client_id, client_identifier, MAX_CLIENT_ID_LEN);
    send(socket_fd, &alive_message, sizeof(alive_message), MSG_DONTWAIT);

    // create listener process
    pid_t listener_pid = fork();
    if (listener_pid < 0)
        perror("fork listener");
    else if (listener_pid == 0) {
        while (!should_close) {
            request_message_t message;
            // receive message from server
            if(recv(socket_fd, &message, sizeof(message), MSG_WAITALL) < 0) {
                perror("recv");
                continue;

            }
            // handle message
            switch(message.request_type) {
                case LIST:
                // print list of clients
                    for (int i = 0; i < message.payload.list.list_length; i++) {
                        printf("%s\n", message.payload.list.identifiers_list[i]);
                    }
                    break;
                case TOALL:
                    printf("TOALL FROM: %s: %s\n", message.sender_client_id, message.payload.to_all);
                    break;
                case TOONE:
                    printf("TOONE FROM: %s: %s\n", message.sender_client_id, message.payload.to_one.message);
                    break;
                case ALIVE:
                    send(socket_fd, &alive_message, sizeof(alive_message), MSG_DONTWAIT);
                    break;
                default:
                    printf("Invalid response type! \n");
                    break;
            }
        }
    } else {
        // main client process to send messages
        char* request_type_input_buffer = NULL;
        while (!should_close) {
            // read request type from user
            if(scanf("%ms", &request_type_input_buffer) == 1) {
                request_message_t message;
                // fill message with request type and sender client id
                strncpy(message.sender_client_id, client_identifier, MAX_CLIENT_ID_LEN);
                
                // handle request type
                if (strncmp(request_type_input_buffer, "LIST", 4) == 0){
                    message.request_type = LIST;

                    // send message to server
                    if(send(socket_fd, &message, sizeof(message), MSG_DONTWAIT) < 0)
                        printf("err sendd");
                } else if (strncmp(request_type_input_buffer, "2ALL", 4) == 0) {
                    message.request_type = TOALL;
                    // read message from user
                    scanf(MAX_MESSAGE_STRING_LEN_SCANF_STR, message.payload.to_all);
                    send(socket_fd, &message, sizeof(message), MSG_DONTWAIT);
                } else if (strncmp(request_type_input_buffer, "2ONE", 4) == 0) {
                    message.request_type = TOONE;
                    // read target client id and message from user
                    scanf(MAX_CLIENT_ID_LEN_SCANF_STR, message.payload.to_one.target_client_id);
                    scanf(MAX_MESSAGE_STRING_LEN_SCANF_STR, message.payload.to_one.message);
                    send(socket_fd, &message, sizeof(message), MSG_DONTWAIT);
                } else {
                    printf("Invalid request type! \n");
                }

                // free buffer
                free(request_type_input_buffer);
            } else 
                perror("scanf input");
            
        }

        // send stop message to server
        request_message_t stop_message = {
            .request_type = STOP
        };
        strncpy(stop_message.sender_client_id, client_identifier, MAX_CLIENT_ID_LEN);
        send(socket_fd, &stop_message, sizeof(stop_message), MSG_DONTWAIT);
    }

    close(socket_fd);
  
    return 0;
}
