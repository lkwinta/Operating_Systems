#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv){
    if (argc == 2){
        char* filename1 = argv[1];
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0){
//  zamknij deskryptor do zapisu i wykonaj program sort na filename1
//  w przypadku błędu zwróć 3
            if (close(fd[1]) < 0) {
                perror("close pipe");
                exit(-1);
            }
            int status = execlp("sort", "sort", filename1, NULL);
            if(status < 0) {
                exit(3);
            }
		  
//  koniec
        }else{
            close(fd[0]);
        }
    }
    else if (argc==3){
        char* filename1 = argv[1];
        char* filename2 = argv[2];
        int fd[2];
//  otwórz plik filename2 z prawami dostępu rwxr--r--, 
//  jeśli plik istnieje otwórz go i usuń jego zawartość

        // jesli plik nie bedzie istnial zostanie stworzony
        // jesli bedzie istnial to zostanie otworzony i wyyczyszczony (brak O_APPEND)
        int fd_filename2 = open(filename2, O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
        if(fd_filename2 < 0) {
            perror("open file");
            return - 1;
        }

//  koniec
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0){
//  zamknij deskryptor do zapisu,
//  przekieruj deskryptor standardowego wyjścia na deskryptor pliku filename2 i zamknij plik,
//  wykonaj program sort na filename1
//  w przypadku błędu zwróć 3.
            if(close(fd[1]) < 0) {
                perror("close pipe");
                exit(-1);
            }

            if(dup2(fd_filename2, STDOUT_FILENO) < 0){
                perror("dup2");
                exit(-1);
            }

            close(fd_filename2);

            int status = execlp("sort", "sort", filename1, NULL);
            if(status < 0) {
                exit(3);
            }
//  koniec
        }else{
            close(fd[0]);
        }
    }
    else
        printf("Wrong number of args! \n");

    return 0;
}