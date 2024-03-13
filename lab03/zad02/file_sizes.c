#include <stdio.h>
#include <dirent.h>

#define PATH_BUFFER_SIZE 1024

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - path to the directory to be searched
*/
int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: reverse <path_to_directory>\n");
        return -1;
    }

    DIR* dir_handle = opendir(argv[1]);
    if(dir_handle == NULL){
        printf("Could not open directory at path: %s \n", argv[1]);
        return -1;
    }

    long long total_size = 0;
    struct dirent* dir_file = readdir(dir_handle);
    char path_buffer[PATH_BUFFER_SIZE] = "sadasdadadad";
    printf("%s \n", path_buffer);
    
    while(dir_file != NULL){
        printf("%s -> %c\n", dir_file->d_name, dir_file->d_type);
        
        dir_file = readdir(dir_handle);
    }

    closedir(dir_handle);
    return 0;
}