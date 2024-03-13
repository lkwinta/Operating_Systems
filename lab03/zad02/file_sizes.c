#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

//define buffer for path, guarantees to be filled with 0
#define PATH_BUFFER_SIZE 1024
char path_buffer[PATH_BUFFER_SIZE];

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - path to the directory to be searched
*/
int main(int argc, char** argv) {
    // if we didn't get 2 arguments we cannot procceed
    if(argc < 2) {
        printf("Usage: reverse <path_to_directory>\n");
        return -1;
    }

    // try to open directory, if it failed, print info and exit
    DIR* dir_handle = opendir(argv[1]);
    if(dir_handle == NULL){
        printf("Could not open directory at path: %s \n", argv[1]);
        return -1;
    }

    // construct begining of the path file buffer
    size_t dir_path_length = strlen(argv[1]);

    //check if path will fit in the buffer
    if(dir_path_length + 1 >= PATH_BUFFER_SIZE - 1){
        printf("Given path is too long! \n");
        return -1;
    }
    memcpy(path_buffer, argv[1], dir_path_length);

    // add / if it was not added in initial path
    if(path_buffer[dir_path_length - 1] != '/'){
        path_buffer[dir_path_length] = '/';
        dir_path_length += 1;
    }

    // declare stat structure, it will be filled for each file
    struct stat file_status; 

    // variable for accumulating sum of file sizes in directory
    unsigned long long total_size = 0;

    // read first directory
    struct dirent* dir_file = readdir(dir_handle);

    // check if there is more files to procceed in the directory
    while(dir_file != NULL){
        size_t file_name_length = strlen(dir_file->d_name);

        // raport error if file path does not fit in the path buffer
        if(dir_path_length + file_name_length >= PATH_BUFFER_SIZE - 1){
            printf("Path to the file doesn't feet in 1024-bytes buffer!");
            return -1;
        }
        
        // construct total file path buffer
        memcpy(&path_buffer[dir_path_length], dir_file->d_name, file_name_length);
        path_buffer[dir_path_length + file_name_length] = 0;

        // fill stat structure
        stat(path_buffer, &file_status);

        // check if file is not a directory
        if(!S_ISDIR(file_status.st_mode)){
            //if it is not print out file size and name and accumulate bytes count
            printf("%8ld %s\n",file_status.st_size, path_buffer);
            total_size += file_status.st_size;
        }

        // read next dir
        dir_file = readdir(dir_handle);
    }

    // print out total count of bytes
    printf("%llu total\n", total_size);

    // remember to close directory
    closedir(dir_handle);
    return 0;
}