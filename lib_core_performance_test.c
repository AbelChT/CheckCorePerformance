//
// Created by abel (https://github.com/AbelChT) on 25/1/20.
//

#include "lib_core_performance_test.h"

/**
 * Set total affinity to CPU
 * @param cpu_where_set cpu where affinity will be set
 * @param previous_mask mask before affinity
 */
int set_affinity_to_cpu(unsigned long cpu_where_set, cpu_set_t *previous_mask) {
    long n_processors = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_where_set >= n_processors || sched_getaffinity(0, sizeof(cpu_set_t), previous_mask) == -1) {
        return -1;
    } else {
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(cpu_where_set, &mask);
        return sched_setaffinity(0, sizeof(cpu_set_t), &mask);
    }
}

/**
 * Get the actual usage statics
 * @param measures
 * @return
 */
int get_actual_usage_statics(struct usage_statics *measures) {
    struct rusage usage;
    int success = getrusage(RUSAGE_SELF, &usage);
    if (success == 0) {
        measures->user_time_ms = usage.ru_utime.tv_usec;
        measures->user_time_s = usage.ru_utime.tv_sec;
        measures->system_time_ms = usage.ru_stime.tv_usec;
        measures->system_time_s = usage.ru_stime.tv_sec;
        measures->involuntary_context_switches = usage.ru_nivcsw;
        measures->voluntary_context_switches = usage.ru_nvcsw;
        return 0;
    } else {
        return -1;
    }
}

/**
 * Return the number of CPU
 */
long get_number_of_cpu() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

/**
 * Start the measure system to test CPU cpu_id_to_test
 * @param cpu_id_to_test cpu to test
 * @param measures request
 */
int start_measure_system(unsigned long cpu_id_to_test, struct measure_system_request *measures) {
    if (set_affinity_to_cpu(cpu_id_to_test, &measures->initial_mask) != -1) {
        if (get_actual_usage_statics(&measures->statics) != -1) {
            measures->statics.elapsed_clock = clock();
            return 0;
        } else {
            sched_setaffinity(0, sizeof(cpu_set_t), &measures->initial_mask);
            return -1;
        }
    } else {
        return -1;
    }
}

/**
 * Finish the measure system
 * @param initial_measurements request
 * @param final_statics final statics
 */
int finish_measure_system(struct measure_system_request *initial_measurements, struct usage_statics *final_statics) {
    struct usage_statics actual_statics;
    int return_value;

    if (get_actual_usage_statics(&actual_statics) != -1) {
        actual_statics.elapsed_clock = clock();
        final_statics->elapsed_clock = actual_statics.elapsed_clock - initial_measurements->statics.elapsed_clock;
        final_statics->involuntary_context_switches = actual_statics.involuntary_context_switches -
                                                      initial_measurements->statics.involuntary_context_switches;
        final_statics->voluntary_context_switches = actual_statics.voluntary_context_switches -
                                                    initial_measurements->statics.voluntary_context_switches;
        final_statics->system_time_s = actual_statics.system_time_s -
                                       initial_measurements->statics.system_time_s;
        final_statics->user_time_s = actual_statics.user_time_s -
                                     initial_measurements->statics.user_time_s;
        if (actual_statics.system_time_ms < initial_measurements->statics.system_time_ms) {
            final_statics->system_time_s = final_statics->system_time_s - 1;
            final_statics->system_time_ms = 1000000L + actual_statics.system_time_ms -
                                            initial_measurements->statics.system_time_ms;
        } else {
            final_statics->system_time_ms = actual_statics.system_time_ms -
                                            initial_measurements->statics.system_time_ms;
        }
        if (actual_statics.user_time_ms < initial_measurements->statics.user_time_ms) {
            final_statics->user_time_s = final_statics->user_time_s - 1;
            final_statics->user_time_ms = 1000000L + actual_statics.user_time_ms -
                                          initial_measurements->statics.user_time_ms;
        } else {
            final_statics->user_time_ms = actual_statics.user_time_ms -
                                          initial_measurements->statics.user_time_ms;
        }
        return_value = 0;
    } else {
        return_value = -1;
    }

    sched_setaffinity(0, sizeof(cpu_set_t), &initial_measurements->initial_mask);
    return return_value;
}