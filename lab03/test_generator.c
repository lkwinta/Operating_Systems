/**
 * Simple util to generate random files with given number of bytes
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - number of bytes to generate
 * argv[2] - output file path
*/
int main(int argc, char** argv) {
    // if we didn't get 3 arguments we cannot procceed
    if(argc < 3){
        printf("Usage: test_generator <number_of_characters> <output_file_path> !\n");
        return -1;
    }

     // try to open input file, stop program if failed
    FILE* file = fopen(argv[2], "w");
    if(file == NULL){
        printf("Failed to open file: %s!\n", argv[2]);
        return -1;
    }

    //convert string argument into long
    long number_of_files = strtol(argv[1], NULL, 10);
    
    //generate wanted amount of bytes to the file
    for(int i = 0; i < number_of_files; i++) {
        fputc(rand()%('z' - 'a' + 1) + 'a', file);
    }

    //close output file
    fclose(file);
}