//
// Created by abel (https://github.com/AbelChT) on 25/1/20.
//

#pragma once

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/resource.h>
#include <sched.h>
#include <unistd.h>
#include <time.h>

/**
 * Information about usage
 */
struct usage_statics {
    long user_time_ms, user_time_s, system_time_ms, system_time_s, involuntary_context_switches, voluntary_context_switches;
    double elapsed_clock;
};

/**
 * Request
 */
struct measure_system_request {
    struct usage_statics statics;
    cpu_set_t initial_mask;
};

/**
 * Return the number of CPU
 */
long get_number_of_cpu();

/**
 * Start the measure system to test CPU cpu_id_to_test
 * @param cpu_id_to_test cpu to test
 * @param measures request
 * @return On error return -1 and 0 on success
 */
int start_measure_system(unsigned long cpu_id_to_test, struct measure_system_request *measures);

/**
 * Finish the measure system
 * @param initial_measurements request
 * @param final_statics final statics
   * @return On error return -1 and 0 on success
 */
int finish_measure_system(struct measure_system_request *initial_measurements, struct usage_statics *final_statics);
