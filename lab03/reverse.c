#include <stdio.h>

#define BUFFER_SIZE 1024

void reverse_byte_by_byte(FILE* input_file, FILE* output_file){
    fseek(input_file, 0, SEEK_END);

    while(ftell(input_file) > 0){
        fseek(input_file, -1, SEEK_CUR);
        fprintf(output_file, "%c", getc(input_file));
    
        fseek(input_file, -1, SEEK_CUR);
    } 
}

void reverse_by_blocks(FILE* input_file, FILE* output_file){
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    char buffer[BUFFER_SIZE];
    long bytes_left = file_size;
    while(bytes_left > 0){
        long bytes_to_read = bytes_left > BUFFER_SIZE ? BUFFER_SIZE : bytes_left;
        fread(buffer, 1, bytes_to_read, input_file);
        for(int i = bytes_to_read - 1; i >= 0; i--){
            fprintf(output_file, "%c", buffer[i]);
        }
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

    reverse_byte_by_byte(input_file, output_file);

    fclose(input_file);
    fclose(output_file);
}