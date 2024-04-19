#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "protocol_specs.h"

int main() {
    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(message_t),
        .mq_maxmsg = 10
    };

    mqd_t mq_descriptor = mq_open(SERVER_QUEUE_NAME, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR, &attributes);
    if(mq_descriptor < 0)
        perror("mq_open server");

    message_t receive_message;

    mqd_t client_queues[MAX_CLIENTS_COUNT];
    int registered_clients = 0;

    while(1) {
        mq_receive(mq_descriptor, (char*)&receive_message, sizeof(receive_message), NULL);
        switch(receive_message.type) {
            case INIT:
                client_queues[registered_clients] = mq_open(receive_message.text, O_RDWR, S_IRUSR | S_IWUSR, NULL);
                if(client_queues[registered_clients] < 0)
                    perror("mq_open client");

                message_t send_message = {
                    .type = IDENTIFIER,
                    .identifier = registered_clients
                };

                mq_send(client_queues[registered_clients], (char*)&send_message, sizeof(send_message), 10);
                registered_clients++;
                break;
            case MESSAGE_TEXT:
                for (int identifier = 0; identifier < registered_clients; identifier++){
                    if(identifier != receive_message.identifier){
                        mq_send(client_queues[identifier], (char*)&receive_message, sizeof(receive_message), 10);
                    }
                }
                break;

            default:
                printf("Unexpected message type in server queue: %d \n", receive_message.type);
                break;
        }
    }
}