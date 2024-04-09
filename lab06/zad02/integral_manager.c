#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "integral_pipe_specs.h"

int main() {
    integral_range_t range;


    /**
     * open named pipes for communication
    */
    int input_pipe_fd = open(IN_PIPE_NAME, O_WRONLY);
    int output_pipe_fd = open(OUT_PIPE_NAME, O_RDONLY);

    double integral_result;

    /**
     * listen on std input for integral range specs and write them to the pipe, later wait for the calculated result
    */
    while(1) {
        printf("Integral range start: \n");
        if(scanf("%lf", &range.range_start) < 0) {
            printf("Failed to read from the stdin\n");
            return -1;
        }
        printf("Integral range stop: \n");
        if(scanf("%lf", &range.range_stop) < 0) {
            printf("Failed to read from the stdin\n");
            return -1;
        }
        printf("Integral number of intervals: \n");
        if(scanf("%llu", &range.number_of_intervals) < 0) {
            printf("Failed to read from the stdin\n");
            return -1;
        }

        if(write(input_pipe_fd, &range, sizeof(range)) < 0) {
            printf("Failed to write to the pipe\n");
            return -1;
        }
        if(read(output_pipe_fd, &integral_result, sizeof(integral_result)) < 0) {
            printf("Failed to read from the pipe\n");
            return -1;
        }

        printf("Integral result: %lf\n\n", integral_result);
    }

    /**
     * close the pipes
    */
    close(input_pipe_fd);
    close(output_pipe_fd);

    return 0;
}