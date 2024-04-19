#ifndef PROTOCOL_SPECS_H
#define PROTOCOL_SPECS_H

#define MESSAGE_BUFFER_SIZE 2048
#define SERVER_QUEUE_NAME "/simple_chat_server_queue"

#define CLIENT_QUEUE_NAME_SIZE 40
#define MAX_CLIENTS_COUNT 40

typedef enum {
    INIT, 
    IDENTIFIER,
    MESSAGE_TEXT
} message_type_t;

typedef struct {
    message_type_t type;

    int identifier;
    char text[MESSAGE_BUFFER_SIZE];
} message_t;

#endif //PROTOCOL_SPECS_H