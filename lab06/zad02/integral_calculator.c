#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "integral_pipe_specs.h"

#define min(a, b) (a < b ? a : b)


double f(double x){
    return 4/(x*x + 1);
}

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

void unlink_pipes(){
    unlink(in_pipe_name);
    unlink(out_pipe_name);
}

int main() {
    if(mkfifo(in_pipe_name, S_IRWXU) != 0) {
        printf("Failed to create input pipe");
        return -1;
    }
    if(mkfifo(out_pipe_name, S_IRWXU) != 0) {
        printf("Failed to create output pipe");
        unlink_pipes();
        return -1;
    }

    int input_pipe_fd = open(in_pipe_name, O_RDONLY);
    if(input_pipe_fd < 0){
        printf("Failed to open input pipe");
        unlink_pipes();
        return -1;
    }

    int output_pipe_fd = open(out_pipe_name, O_WRONLY);
    if(output_pipe_fd < 0){
        printf("Failed to open output pipe");
        unlink_pipes();
        return -1;
    }

    integral_range_t range;
    double integral_result;

    while(read(input_pipe_fd, &range, sizeof(range)) > 0) {
        integral_result = calculate_integral(&range, f);
        write(output_pipe_fd, &integral_result, sizeof(integral_result));
    }

    close(input_pipe_fd);
    close(output_pipe_fd);
    unlink_pipes();
    return 0;
}