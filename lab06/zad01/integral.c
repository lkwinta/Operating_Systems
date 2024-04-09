#include <printf.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define min(a, b) (a < b ? a : b)

typedef unsigned long long ull;

///////////////////////////////////////////////////////////////////
// integral parameters
double range_start = 0.0;
double range_stop = 1.0;

double f(double x){
    return 4/(x*x + 1);
}
///////////////////////////////////////////////////////////////////

/**
 * @brief function calculating integral in given range
 * 
 * @param interval_start start of the interval
 * @param interval_stop end of the interval
 * @param fun pointer to the function that will be integrated
 * @param interval_width width of the single rectangle
 * @param number_of_intervals number of rectangles that will be used to calculate integral
 */
double calculate_integral(double interval_start, double interval_stop, double (*fun)(double), double interval_width, ull number_of_intervals){
    if(interval_stop - interval_start < interval_width)
        return fun((interval_start + interval_stop)/ 2.0)*(interval_stop - interval_start);

    double sum = 0.0;
    for(ull i = 0; i < number_of_intervals; i++){
        sum += fun((interval_start + interval_width/2.0));
        interval_start += interval_width;
    }

    return sum * interval_width;
}

/**
 * argv[0] - execution command - always passed as default
 * argv[1] - width of single rectangle that constructs 0...1 range
 * argv[2] - number of processes that should be spawn to parallel the work
*/
int main(int argc, char** argv) {
     // if we didn't get 3 arguments we cannot proceed
    if(argc < 3) {
        printf("Usage: %s <interval_width> <number_of_processes>\n", argv[0]);
        return -1;
    }

    // convert precision to double variable
    double interval_width = strtod(argv[1], NULL);

    // convert number of processes to long variable
    long num_of_processes = strtol(argv[2], NULL, 10);
    
    /**
     * check if there is enough intervals for all the processes
    */
    if(ceil((range_stop - range_start)/interval_width) < num_of_processes) {
        printf("To much processes needed for given interval range");
        return -1;
    }

    /**
     * calculate number of intervals per process
    */
    ull total_intervals_count = (ull)ceil((double)(range_stop - range_start)/interval_width);
    ull intervals_per_process = total_intervals_count/num_of_processes;


    double interval_start = range_start;
    double interval_stop = range_start;

    /**
     * create file descriptors for pipes
    */
    int pipes_fd[num_of_processes][2];

    /**
     * dispatch task for each process 
    */
    for(int i = 0; i < num_of_processes; i++){
        interval_stop = min(range_stop, interval_start + intervals_per_process*interval_width);

        // create pipe for integral result
        if(pipe(pipes_fd[i]) < 0) {
            printf("Failed to create pipe\n");
            return -1;
        }
        // create new process
        pid_t pid = fork();

        if(pid == 0){
            // close output of the pipe in child process
            close(pipes_fd[i][0]);

            // calculate integral
            double integral_result = calculate_integral(interval_start, interval_stop, f, interval_width, intervals_per_process);

            //write result to the pipe
            if(write(pipes_fd[i][1], &integral_result, sizeof(integral_result)) < 0) {
                printf("Failed to write to the pipe\n");
                return -1;
            }

            exit(0);
        }

        // close input of the pipe in parent process
        close(pipes_fd[i][1]);

        interval_start = interval_stop;
    }

    double sum = 0.0;

    /**
     * try to read integral results from every pipe 
     * and sum them up, read will block parent process until
     * there is data in pipe to read
    */
    for(int i = 0; i < num_of_processes; i++){
        double integral_result;
        if(read(pipes_fd[i][0], &integral_result, sizeof(integral_result)) < 0) {
            printf("Failed to read from the pipe\n");
            return -1;
        }
        sum += integral_result;
    } 

    /**
     * print final result
    */
    printf("total result: %lf\n", sum);

    return 0;
}