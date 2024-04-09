#ifndef INTEGRAL_PIPE_SPECS_H
#define INTEGRAL_PIPE_SPECS_H

typedef unsigned long long ull;

/** name of the pipes */
#define IN_PIPE_NAME "int_calc_in.fifo"
#define OUT_PIPE_NAME "int_calc_out.fifo"

/** struct for the data to be pushed */
typedef struct {
    double range_start;
    double range_stop;

    ull number_of_intervals;
} integral_range_t;

#endif //INTEGRAL_PIPE_SPECS_H