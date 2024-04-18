#ifndef PROTOCOL_SPECS_H
#define PROTOCOL_SPECS_H

#define MESSAGE_BUFFER_SIZE 2048
#define SERVER_QUEUE_NAME "/simple_chat_server_queue"

typedef enum {
    INIT, 
    IDENTIFIER,
    MESSAGE_TEXT
} message_type_t;

typedef struct {
    message_type_t type;

    union {
        char text[MESSAGE_BUFFER_SIZE];
        int number;
    } payload;
    
} message_t;

#endif //PROTOCOL_SPECS_H