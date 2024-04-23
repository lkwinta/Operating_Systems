#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "protocol_specs.h"

/** signal handler to exit from loop */
volatile bool should_close = false;

void SIGNAL_handler(int signum) {
    should_close = true;
}

// helper macro for getting minimum of two values
#define MIN(a, b) (a < b ? a : b)

int main() {
    /* create unique identifier for client queue */
    /* I set name to be associated with clients pid because it increases chances of generating unique queue name */
    pid_t pid = getpid();
    char queue_name[CLIENT_QUEUE_NAME_SIZE] = {0};
    sprintf(queue_name, "/simple_chat_client_queue_%d", pid);

    /**
     * Fill in structure speicifing options for creation of client queue
    */
    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(message_t),
        .mq_maxmsg = 10
    };
    
    /**
     * Create client queue with options for server -> client communication
     *  - O_RDWR - open for reading and writing
     *  - O_CREAT - create queue if it does not exist
     *  - S_IRUSR | S_IWUSR - set permissions for user to read and write
    */
    mqd_t mq_client_descriptor = mq_open(queue_name, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR, &attributes);
    if(mq_client_descriptor < 0)
        perror("mq_open client");

    /**
     * Try opening server queue for client -> server communication
    */
    mqd_t mq_server_descriptor = mq_open(SERVER_QUEUE_NAME, O_RDWR, S_IRUSR | S_IWUSR, NULL);
    if(mq_server_descriptor < 0) {
        printf("Most likely server not opened!\n");
        perror("mq_open server");
    }

    /**
     * Setup INIT message with client queue name
    */
    message_t message_init = {
        .type = INIT,
        .identifier = -1
    };

    // copy queue name to message buffer, MIN is used to prevent buffer overflow
    memcpy(message_init.text, queue_name, MIN(CLIENT_QUEUE_NAME_SIZE - 1, strlen(queue_name)));

    // send INIT message to server
    if(mq_send(mq_server_descriptor, (char*)&message_init, sizeof(message_init), 10) < 0){
        perror("mq_send init");
    }

    // create pipe for communication between parent (writing to server) and child (reading from server) processes
    int to_parent_pipe[2];
    if(pipe(to_parent_pipe) < 0)
        perror("pipe");

    // register signal handler for closing client to all signals
    for (int sig = 1; sig < SIGRTMAX; sig++) {
        signal(sig, SIGNAL_handler);
    }

    // create child process for listening to messages from server
    pid_t listener_pid = fork();
    if (listener_pid < 0)
        perror("fork listener");
    else if (listener_pid == 0) {
        // close reading end of the pipe
        close(to_parent_pipe[0]);
        message_t receive_message;
        while(!should_close) {
            // receive message from server (block until message is received)
            mq_receive(mq_client_descriptor, (char*)&receive_message, sizeof(receive_message), NULL);
            switch(receive_message.type) {
                /* we received standard message from server with text data */
                case MESSAGE_TEXT:
                    printf("Received from id: %d message: %s\n", receive_message.identifier, receive_message.text);
                    break;
                /* we received client identifier from server */
                case IDENTIFIER:
                    printf("Received identifier from server: %d\n", receive_message.identifier);
                    // write client identifier to parent process
                    write(to_parent_pipe[1], &receive_message.identifier, sizeof(receive_message.identifier));
                    break;
                default:
                    printf("Unknown message type in client queue: %d", receive_message.type);
                    break;
            }
        }
        printf("Exiting from receive loop\n");
        exit(0);
    } else {
        //close writing end of the pipe
        close(to_parent_pipe[1]);
        int identifier = -1;

        // wait and read client identifier from child process
        if(read(to_parent_pipe[0], &identifier, sizeof(identifier)) < 0)
            perror("read identifier");

        char* buffer = NULL;
        while(!should_close) {
            // get message queue attributes
            mq_getattr(mq_server_descriptor, &attributes);
            // check if we can send more messages to server
            if(attributes.mq_curmsgs >= attributes.mq_maxmsg) {
                printf("Server is busy, please wait\n");
                continue;
            }
            
            /**
             *  save version of scanf - allocates memory with string of given length thanks to %ms parameter specifier, 
             *  needs to be free after
             *  prevent buffer overflow
             * */
            if(scanf("%ms", &buffer) == 1) {
                // setup message for sending to server
                message_t send_message = {
                    .type = MESSAGE_TEXT,
                    .identifier = identifier
                };
                // copy message to message buffer, MIN is used to prevent buffer overflow
                memcpy(send_message.text, buffer, MIN(MESSAGE_BUFFER_SIZE - 1, strlen(buffer)));

                // send message to server
                mq_send(mq_server_descriptor, (char*)&send_message, sizeof(send_message), 10);

                // free memory allocated by scanf
                free(buffer);
                buffer = NULL;
            } else
                perror("scanf input");
        }

        printf("Exiting from sending loop\n");

        /* If we did make connection notify server about closing */
        if(identifier != -1){
            /** Prepare notification for the server that client is closing */
            message_t message_close = {
                .type = CLIENT_CLOSE,
                .identifier = identifier
            };
            /* Notify server that client is closing */
            mq_send(mq_server_descriptor, (char*)&message_close, sizeof(message_close), 10);
        }

        /* Close quees  */
        mq_close(mq_server_descriptor);
        mq_close(mq_client_descriptor);

        /** Delete created queues file descriptor */
        mq_unlink(queue_name);
    }
}