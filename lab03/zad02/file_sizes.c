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
    if(argc < 2) {
        printf("Usage: reverse <path_to_directory>\n");
        return -1;
    }

    DIR* dir_handle = opendir(argv[1]);
    if(dir_handle == NULL){
        printf("Could not open directory at path: %s \n", argv[1]);
        return -1;
    }

    unsigned long long total_size = 0;
    struct dirent* dir_file = readdir(dir_handle);

    size_t dir_path_length = strlen(argv[1]);
    if(dir_path_length + 1 >= PATH_BUFFER_SIZE - 1){
        printf("Given path is too long! \n");
        return -1;
    }
    memcpy(path_buffer, argv[1], dir_path_length);

    if(path_buffer[dir_path_length - 1] != '/'){
        path_buffer[dir_path_length] = '/';
        dir_path_length += 1;
    }

    struct stat file_status; 
    
    while(dir_file != NULL){
        size_t file_name_length = strlen(dir_file->d_name);
        if(dir_path_length + file_name_length >= PATH_BUFFER_SIZE - 1){
            printf("Path to the file doesn't feet in 1024-bytes buffer!");
            return -1;
        }
        
        memcpy(&path_buffer[dir_path_length], dir_file->d_name, file_name_length);
        path_buffer[dir_path_length + file_name_length] = 0;

        stat(path_buffer, &file_status);

        if(!S_ISDIR(file_status.st_mode)){
            printf("%8ld %s\n",file_status.st_size, path_buffer);
            total_size += file_status.st_size;
        }
        
        dir_file = readdir(dir_handle);
    }

    printf("%llu total\n", total_size);

    closedir(dir_handle);
    return 0;
}