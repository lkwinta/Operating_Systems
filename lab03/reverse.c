#include <stdio.h>

#define BUFFER_SIZE 1024

void reverse_byte_by_byte(FILE* input_file, FILE* output_file){
    fseek(input_file, -1, SEEK_END);
    long bytes_to_read = ftell(input_file) + 1;

    while(bytes_to_read > 0){
        fprintf(output_file, "%c", getc(input_file));
        bytes_to_read--;

        fseek(input_file, -2, SEEK_CUR);
    } 
}

void reverse_by_blocks(FILE* input_file, FILE* output_file){
    fseek(input_file, 0, SEEK_END);
    long bytes_left = ftell(input_file);

    char buffer[BUFFER_SIZE];
    
    while(bytes_left > 0){
        long bytes_to_fseek = ftell(input_file) < BUFFER_SIZE ? ftell(input_file) : BUFFER_SIZE;
        fseek(input_file, -bytes_to_fseek, SEEK_CUR);

        long bytes_to_read = bytes_left > BUFFER_SIZE ? BUFFER_SIZE : bytes_left;
        fread(buffer, 1, bytes_to_read, input_file);

        for(int i = bytes_to_read - 1; i >= 0; i--){
            fprintf(output_file, "%c", buffer[i]);
        }

        fseek(input_file, -BUFFER_SIZE, SEEK_CUR);
        
        bytes_left -= bytes_to_read;
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: reverse <input_file> <output_file>\n");
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }

    FILE* output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[2]);
        return 1;
    }

    reverse_by_blocks(input_file, output_file);

    fclose(input_file);
    fclose(output_file);
}