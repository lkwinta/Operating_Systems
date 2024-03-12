#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if(argc < 3){
        printf("Usage: test_generator <number_of_characters> <output_file_path> !\n");
        return -1;
    }

    FILE* file = fopen(argv[2], "w");
    if(file == NULL){
        printf("Failed to open file: %s!\n", argv[2]);
        return -1;
    }

    long number_of_files = strtol(argv[1], NULL, 10);
    
    for(int i = 0; i < number_of_files; i++) {
        fputc(rand()%('z' - 'a' + 1) + 'a', file);
    }

    fclose(file);
}