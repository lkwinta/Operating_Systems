#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "protocol_specs.h"

int main() {
    /* create unique identifier for client queue */
    pid_t pid = getpid();
    char queue_name[40] = {0};
    sprintf(queue_name, "/simple_chat_client_queue_%d", pid);

    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(message_t),
        .mq_maxmsg = 10
    };

    mqd_t mq_client_descriptor = mq_open(queue_name, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR, &attributes);
    if(mq_client_descriptor < 0) {
        perror("mq_open client");
    }

    mqd_t mq_server_descriptor = mq_open(SERVER_QUEUE_NAME, O_RDWR, S_IRUSR | S_IWUSR, NULL);
    if(mq_server_descriptor < 0) {
        perror("mq_open server");
    }

    message_t message = {
        .type = INIT
    };
    memcpy(message.payload.text, queue_name, strlen(queue_name));

    mq_send(mq_server_descriptor, (char*)&message, sizeof(message), 10);

    while(1) {
        mq_getattr(mq_client_descriptor, &attributes);
    }
}