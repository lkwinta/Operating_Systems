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
    if(mq_descriptor < 0) {
        perror("mq_open");
    }

    message_t message;

    while(1) {
        mq_receive(mq_descriptor, (char*)&message, sizeof(message), NULL);
        printf("client queue: %s\n", message.payload.text);
    }
}