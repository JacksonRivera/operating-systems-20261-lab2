#ifndef SCHEDULER_H
#define SCHEDULER_H

#define PID_MAX_LENGTH 32
#define QUEUE_LEVELS 3

typedef struct {
    char pid[PID_MAX_LENGTH];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int finish_time;
    int first_response_time;
    int current_queue;
    int slice_remaining;
} Process;

void init_process(Process *process, const char *pid, int arrival_time, int burst_time);
void simulate_mlfq(Process *processes, int count, int boost_interval);
void print_results_table(const Process *processes, int count);
int export_results_csv(const char *filename, const Process *processes, int count);
int calculate_response_time(const Process *process);
int calculate_turnaround_time(const Process *process);
int calculate_waiting_time(const Process *process);
int get_quantum_for_queue(int queue_level);

#endif
