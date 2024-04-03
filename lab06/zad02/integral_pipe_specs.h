#ifndef INTEGRAL_PIPE_SPECS_H
#define INTEGRAL_PIPE_SPECS_H

typedef unsigned long long ull;

const char* in_pipe_name = "int_calc_in.fifo";
const char* out_pipe_name = "int_calc_out.fifo";

typedef struct {
    double range_start;
    double range_stop;

    ull number_of_intervals;
} integral_range_t;

#endif //INTEGRAL_PIPE_SPECS_H