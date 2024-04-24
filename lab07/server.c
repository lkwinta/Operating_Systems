#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <signal.h>

#include "protocol_specs.h"

/** signal handler to exit from loop */
volatile bool should_close = false;

void SIGNAL_handler(int signum) {
    should_close = true;
}

int main() {
    /**
     * Fill in structure speicifing options for creation of client queue
    */
    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(message_t),
        .mq_maxmsg = 10
    };

    /**
     * Create server queue with options for client -> server communication
     *  - O_RDWR - open for reading and writing
     *  - O_CREAT - create queue if it does not exist
     *  - S_IRUSR | S_IWUSR - set permissions for user to read and write
    */
    mqd_t mq_descriptor = mq_open(SERVER_QUEUE_NAME, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR, &attributes);
    if(mq_descriptor < 0)
        perror("mq_open server");

    message_t receive_message;

    /**
     * Array of client queues descriptors
     *  - initialized with -1 to indicate that id is not used
    */
    mqd_t client_queues[MAX_CLIENTS_COUNT];
    for (int i = 0; i < MAX_CLIENTS_COUNT; i++)
        client_queues[i] = -1;


    // register signal handler for closing client to all signals
    for (int sig = 1; sig < SIGRTMAX; sig++) {
        signal(sig, SIGNAL_handler);
    }

    while(!should_close) {
        // receive message from server queue
        mq_receive(mq_descriptor, (char*)&receive_message, sizeof(receive_message), NULL);

        switch(receive_message.type) {
            /* Requested initialization from the client */
            case INIT:
                /* Find first available id */
                int id = 0;
                while(client_queues[id] != -1 && id < MAX_CLIENTS_COUNT) id++;

                /* If all id's are used, we skip initialization for that client */
                if(id == MAX_CLIENTS_COUNT){
                    printf("Max number of clients has connected, can't open another connection\n");
                    continue;
                }

                /* Open client queue with name received from client */
                client_queues[id] = mq_open(receive_message.text, O_RDWR, S_IRUSR | S_IWUSR, NULL);
                if(client_queues[id] < 0)
                    perror("mq_open client");

                /* Send identifier to client */
                message_t send_message = {
                    .type = IDENTIFIER,
                    .identifier = id
                };

                mq_send(client_queues[id], (char*)&send_message, sizeof(send_message), 10);
                printf("Registered connection with client at id: %d\n", id);
                break;
            /* Received standard message to be broadcasted */
            case MESSAGE_TEXT:
                /* Loop over all possible id's and broadcast received message to all connected clients */
                for (int identifier = 0; identifier < MAX_CLIENTS_COUNT; identifier++){
                    if(identifier != receive_message.identifier && identifier != -1){
                        /* Broadcast received message */
                        mq_send(client_queues[identifier], (char*)&receive_message, sizeof(receive_message), 10);
                    }
                }
                break;
            /* Received message from client informing that client has been closed */
            case CLIENT_CLOSE:
                /* Close client queue */
                mq_close(client_queues[receive_message.identifier]);
                /* Mark that id is not used */
                client_queues[receive_message.identifier] = -1;
                printf("Closed connection with client at id: %d\n", receive_message.identifier);
                break;
            default:
                printf("Unexpected message type in server queue: %d \n", receive_message.type);
                break;
        }
    }

    printf("Exiting server\n");

    /**
     * Close all client queues
    */
    for (int i = 0; i < MAX_CLIENTS_COUNT; i++){
        if(client_queues[i] != -1)
            mq_close(client_queues[i]);
    }

    /**
     * Close and unlink server queue
    */
    mq_close(mq_descriptor);
    mq_unlink(SERVER_QUEUE_NAME);

    return 0;
}