#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "protocol_specs.h"

#define MIN(a, b) (a < b ? a : b)

int main() {
    /* create unique identifier for client queue */
    pid_t pid = getpid();
    char queue_name[CLIENT_QUEUE_NAME_SIZE] = {0};
    sprintf(queue_name, "/simple_chat_client_queue_%d", pid);

    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(message_t),
        .mq_maxmsg = 10
    };

    mqd_t mq_client_descriptor = mq_open(queue_name, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR, &attributes);
    if(mq_client_descriptor < 0)
        perror("mq_open client");

    mqd_t mq_server_descriptor = mq_open(SERVER_QUEUE_NAME, O_RDWR, S_IRUSR | S_IWUSR, NULL);
    if(mq_server_descriptor < 0)
        perror("mq_open server");

    message_t message_init = {
        .type = INIT,
        .identifier = -1
    };
    memcpy(message_init.text, queue_name, MIN(CLIENT_QUEUE_NAME_SIZE - 1, strlen(queue_name)));

    if(mq_send(mq_server_descriptor, (char*)&message_init, sizeof(message_init), 10) < 0){
        perror("mq_send init");
    }

    int to_parent_pipe[2];
    if(pipe(to_parent_pipe) < 0)
        perror("pipe");

    pid_t listener_pid = fork();
    
    if (listener_pid < 0)
        perror("fork listener");
    else if (listener_pid == 0) {
        close(to_parent_pipe[0]);
        message_t receive_message;
        while(1) {
            mq_receive(mq_client_descriptor, (char*)&receive_message, sizeof(receive_message), NULL);
            switch(receive_message.type) {
                case MESSAGE_TEXT:
                    printf("Received from id: %d message: %s\n", receive_message.identifier, receive_message.text);
                    break;
                case IDENTIFIER:
                    printf("Received identifier from server: %d\n", receive_message.identifier);
                    write(to_parent_pipe[1], &receive_message.identifier, sizeof(receive_message.identifier));
                    break;
                default:
                    printf("Unknown message type in client queue: %d", receive_message.type);
                    break;
            }
        }
    } else {
        close(to_parent_pipe[1]);
        int identifier = -1;
        read(to_parent_pipe[0], &identifier, sizeof(identifier));

        char* buffer = NULL;
        while(1) {
            //TODO: check if able to send more communicates
            mq_getattr(mq_server_descriptor, &attributes);
            
            // save version of scanf - allocates memory with string of given length thanks to %ms parameter specifier, 
            // needs to be free after
            // prevent buffer overflow
            if(scanf("%ms", &buffer) == 1) {
                message_t send_message = {
                    .type = MESSAGE_TEXT,
                    .identifier = identifier
                };
                memcpy(send_message.text, buffer, MIN(MESSAGE_BUFFER_SIZE - 1, strlen(buffer)));

                mq_send(mq_server_descriptor, (char*)&send_message, sizeof(send_message), 10);

                free(buffer);
                buffer = NULL;
            } else
                perror("scanf input");
        }
    }
}