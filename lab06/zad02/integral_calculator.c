#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "integral_pipe_specs.h"

#define min(a, b) (a < b ? a : b)

/// @brief function to calculate the value of the function at a given point
/// @param x point at which the function should be calculated 
double f(double x){
    return 4/(x*x + 1);
}

/// @brief function to calculate the integral of a given function in a given range
/// @param range range of the integral
/// @param fun function to be integrated
double calculate_integral(integral_range_t* range, double (*fun)(double)){
    double sum = 0.0;
    double interval_width = (range->range_stop - range->range_start)/range->number_of_intervals;

    double interval_start = range->range_start;

    for(ull i = 0; i < range->number_of_intervals; i++){
        sum += fun((interval_start + interval_width/2.0));
        interval_start += interval_width;
    }

    return sum * interval_width;
}

/// @brief function to unlink the pipes - remove them from the filesystem
void unlink_pipes(){
    unlink(IN_PIPE_NAME);
    unlink(OUT_PIPE_NAME);
}

int main() {
    // try to create the named pipes
    if(mkfifo(IN_PIPE_NAME, S_IRWXU) != 0) {
        printf("Failed to create input pipe");
        return -1;
    }
    if(mkfifo(OUT_PIPE_NAME, S_IRWXU) != 0) {
        printf("Failed to create output pipe");
        unlink_pipes();
        return -1;
    }

    // try to open input file descriptor
    int input_pipe_fd = open(IN_PIPE_NAME, O_RDONLY);
    if(input_pipe_fd < 0){
        printf("Failed to open input pipe");
        unlink_pipes();
        return -1;
    }

    // try to open output file descriptor
    int output_pipe_fd = open(OUT_PIPE_NAME, O_WRONLY);
    if(output_pipe_fd < 0){
        printf("Failed to open output pipe");
        unlink_pipes();
        return -1;
    }

    integral_range_t range;
    double integral_result;

    // wait for the input from the input pipe and calculate the integral, then write the result to the output pipe
    while(read(input_pipe_fd, &range, sizeof(range)) > 0) {
        integral_result = calculate_integral(&range, f);
        if(write(output_pipe_fd, &integral_result, sizeof(integral_result)) < 0) {
            printf("Failed to write to output pipe");
            break;
        }
    }

    
    // close and unlink the pipes
    close(input_pipe_fd);
    close(output_pipe_fd);
    unlink_pipes();
    return 0;
}