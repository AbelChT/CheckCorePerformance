/**
 * Author: Abel Chils Trabanco (https://github.com/AbelChT)
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <sys/resource.h>
#include <sched.h>
#include <unistd.h>

void print_system_usage() {
    struct rusage usage;
    int success = getrusage(RUSAGE_SELF, &usage);

    if (success == 0) {
        long user_time_ms = usage.ru_utime.tv_usec;
        long user_time_s = usage.ru_utime.tv_sec;
        long system_time_ms = usage.ru_stime.tv_usec;
        long system_time_s = usage.ru_stime.tv_sec;
        long involuntary_context_switches = usage.ru_nivcsw;
        long voluntary_context_switches = usage.ru_nvcsw;

        printf("Statics -> User time: %li seconds and %li microseconds, system time: %li seconds and %li microseconds, involuntary context switches: %li, voluntary context switches: %li\n",
               user_time_s, user_time_ms, system_time_s, system_time_ms, involuntary_context_switches,
               voluntary_context_switches);

    } else {
        printf("Error on retrieve statics \n");
    }
}

long compute_something(long iteration_number) {
    long number_to_multiply = 3;
    long number_to_sum = 17;

    long dummy_variable = 0;

    for (long i = 0; i < iteration_number; i++) {
        dummy_variable = +i * number_to_multiply + number_to_sum;
    }

    return dummy_variable;
}

void set_affinity_to_cpu(unsigned long cpu_where_set) {
    long n_processors = sysconf(_SC_NPROCESSORS_ONLN);

    if (cpu_where_set >= n_processors) {
        printf("Error invalid cpu for affinity \n");
    } else {
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(cpu_where_set, &mask);

        if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
            printf("Error on setting cpu affinity \n");
        }
    }
}

int main() {
    long iteration_number = 10000000000L;

    set_affinity_to_cpu(3);

    long dummy_result = compute_something(iteration_number);
    print_system_usage();

    printf("Dummy print: %ld\n", dummy_result);

    return 0;
}