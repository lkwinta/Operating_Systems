#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


int main(int argc, char* argv[])
{

 if(argc !=4){
    printf("Not a suitable number of program parameters\n");
    return(1);
 }

    //utworz proces potomny w ktorym wykonasz program ./calc z parametrami argv[1], argv[2] oraz argv[3]
    //odpowiednio jako pierwszy operand, operacja (+-x/) i drugi operand 
    //uzyj tablicowego sposobu przekazywania parametrow do programu 
   pid_t pid = fork();
   if(pid < 0) {
      perror("fork");
      return -1;
   }
   else if(pid == 0) {
      char* args[] = {"calc", argv[1], argv[2], argv[3], NULL};

      int status = execvp("./calc", args);
      exit(status);
   } else {
      int status;
      if(wait(&status) < 0) {
         perror("wait");
         return -1;
      }
      printf("Calc exited with: %d \n", WEXITSTATUS(status));
   }

   return 0;
}
