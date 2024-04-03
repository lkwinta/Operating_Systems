#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "integral_pipe_specs.h"

int main() {
    integral_range_t range;

    int input_pipe_fd = open(in_pipe_name, O_WRONLY);
    int output_pipe_fd = open(out_pipe_name, O_RDONLY);

    double integral_result;

    while(1) {
        printf("Integral range start: \n");
        scanf("%lf", &range.range_start);
        printf("Integral range stop: \n");
        scanf("%lf", &range.range_stop);
        printf("Integral number of intervals: \n");
        scanf("%llu", &range.number_of_intervals);

        write(input_pipe_fd, &range, sizeof(range));
        read(output_pipe_fd, &integral_result, sizeof(integral_result));

        printf("Integral result: %lf\n\n", integral_result);
    }

    close(input_pipe_fd);
    close(output_pipe_fd);

    return 0;
}