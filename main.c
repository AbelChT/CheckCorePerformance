/**
 * Author: Abel Chils Trabanco (https://github.com/AbelChT)
 */
#define _GNU_SOURCE

#include <stdio.h>
#include "lib_core_performance_test.h"

long compute_something(long iteration_number) {
    long number_to_multiply = 3;
    long number_to_sum = 17;

    long dummy_variable = 0;

    for (long i = 0; i < iteration_number; i++) {
        dummy_variable = +i * number_to_multiply + number_to_sum;
    }

    return dummy_variable;
}

int main() {
    long iteration_number = 10000000000L;

    struct measure_system_request initial_measures;
    struct usage_statics final_statics;

    int start_measure_error = start_measure_system(3, &initial_measures);

    if (start_measure_error == -1) {
        printf("Error on start measure\n");
    }

    long dummy_result = compute_something(iteration_number);

    int finish_measure_error = finish_measure_system(&initial_measures, &final_statics);

    if (finish_measure_error == -1) {
        printf("Error on finish measure\n");
    }

    double total_time = (double) final_statics.elapsed_clock / CLOCKS_PER_SEC;

    printf("Statics -> Elapsed time: %lf, User time: %li seconds and %li microseconds, system time: %li seconds and %li microseconds, involuntary context switches: %li, voluntary context switches: %li\n",
           total_time, final_statics.user_time_s, final_statics.user_time_ms, final_statics.system_time_s,
           final_statics.system_time_ms, final_statics.involuntary_context_switches,
           final_statics.voluntary_context_switches);

    printf("Dummy print: %ld\n", dummy_result);

    return 0;
}