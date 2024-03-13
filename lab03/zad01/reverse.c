#include <stdio.h>

#if defined(BYTES)
// reverse function reading file byte by byte
void reverse(FILE* input_file, FILE* output_file){
    //seek to the character next to the end of the file
    fseek(input_file, -1, SEEK_END);
    // get the length of the file
    long bytes_to_read = ftell(input_file) + 1;

    // while we didn't read whole file we read next byte
    while(bytes_to_read > 0){
        // read current byte
        fprintf(output_file, "%c", getc(input_file));
        bytes_to_read--; //mark byte as read

        //seek to bytes back from current position
        //2 because if we read one file we move the pointer one to the front, so we have to move it 2 bytes backward
        // to read next byte
        fseek(input_file, -2, SEEK_CUR);
    } 
}

#elif !defined(BYTES)
//reverse function that reverses file in 1024 byte blocks
#define BUFFER_SIZE 1024
void reverse(FILE* input_file, FILE* output_file){
    // seek to the end of the file and get total amount of bytes in file
    fseek(input_file, 0, SEEK_END);
    long bytes_left = ftell(input_file);

    // declare buffer for read bytes
    char buffer[BUFFER_SIZE];
    
    while(bytes_left > 0){
        //calculate how many bytes we can read in iteration
        // if we have left less then BUFFER_SIZE bytes left
        // we can't call seek with BUFFER_SIZE because it won't move pointer
        // so we seek with only available bytes to read
        // so we move to the begining of the file
        long bytes_to_read = bytes_left > BUFFER_SIZE ? BUFFER_SIZE : bytes_left;
        fseek(input_file, -bytes_to_read, SEEK_CUR);

        //we read calculated amount of bytes to the buffer
        size_t bytes_read = fread(buffer, sizeof(char), bytes_to_read, input_file);

        //iterate over the buffer and reverse it contents
        char c;
        for(int i = 0; i < bytes_read/2; i++){
            c = buffer[i];
            buffer[i] = buffer[bytes_read - 1 - i];
            buffer[bytes_read - 1 - i] = c;
        }

        //we write reversed buffer to the file
        fwrite(buffer, sizeof(char), bytes_read, output_file);

        //we can always seek BUFFER_SIZE hear, either we have read all bytes from the file 
        // or we have more than BUFFER_SIZE bytes to seek
        fseek(input_file, -BUFFER_SIZE, SEEK_CUR);
        
        bytes_left -= bytes_read; //update amount of bytes left
    }
}

#endif

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - input file path
 * argv[2] - output file path
*/
int main(int argc, char** argv) {
    // if we didn't get 3 arguments we cannot procceed
    if (argc != 3) { 
        printf("Usage: reverse <input_file> <output_file>\n");
        return -1;
    }

    // try to open input file, stop program if failed
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return -1;
    }

    // try to open output file, stop program if failed
    FILE* output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[2]);
        return -1;
    }

    // call reverse function
    reverse(input_file, output_file);

    // remember to close files
    // but technically not necessary at the really end of the program - files will be freed automatically
    fclose(input_file);
    fclose(output_file);
}