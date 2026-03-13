#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheduler/scheduler.h"

typedef struct {
    int *items;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

static int init_queue(Queue *queue, int capacity) {
    queue->items = (int *)malloc(sizeof(int) * capacity);
    if (queue->items == NULL) {
        return 0;
    }

    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = capacity;
    return 1;
}

static void destroy_queue(Queue *queue) {
    free(queue->items);
    queue->items = NULL;
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = 0;
}

static int is_queue_empty(const Queue *queue) {
    return queue->size == 0;
}

static int enqueue(Queue *queue, int value) {
    if (queue->size == queue->capacity) {
        return 0;
    }

    queue->items[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
    return 1;
}

static int dequeue(Queue *queue, int *value_out) {
    if (is_queue_empty(queue)) {
        return 0;
    }

    *value_out = queue->items[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return 1;
}

static int find_highest_non_empty_queue(const Queue queues[QUEUE_LEVELS]) {
    int level;

    for (level = 0; level < QUEUE_LEVELS; level++) {
        if (!is_queue_empty(&queues[level])) {
            return level;
        }
    }

    return -1;
}

static void boost_waiting_processes(Queue queues[QUEUE_LEVELS], Process *processes, int count) {
    int level;
    int process_index;

    for (level = 1; level < QUEUE_LEVELS; level++) {
        while (dequeue(&queues[level], &process_index)) {
            processes[process_index].current_queue = 0;
            processes[process_index].slice_remaining = get_quantum_for_queue(0);
            enqueue(&queues[0], process_index);
        }
    }

    for (process_index = 0; process_index < count; process_index++) {
        if (processes[process_index].remaining_time > 0 &&
            processes[process_index].arrival_time >= 0 &&
            processes[process_index].current_queue != 0) {
            processes[process_index].current_queue = 0;
        }
    }
}

void init_process(Process *process, const char *pid, int arrival_time, int burst_time) {
    strncpy(process->pid, pid, PID_MAX_LENGTH - 1);
    process->pid[PID_MAX_LENGTH - 1] = '\0';

    process->arrival_time = arrival_time;
    process->burst_time = burst_time;
    process->remaining_time = burst_time;
    process->start_time = -1;
    process->finish_time = -1;
    process->first_response_time = -1;
    process->current_queue = 0;
    process->slice_remaining = get_quantum_for_queue(0);
}

int get_quantum_for_queue(int queue_level) {
    if (queue_level == 0) {
        return 2;
    }

    if (queue_level == 1) {
        return 4;
    }

    return 8;
}

int calculate_response_time(const Process *process) {
    return process->first_response_time - process->arrival_time;
}

int calculate_turnaround_time(const Process *process) {
    return process->finish_time - process->arrival_time;
}

int calculate_waiting_time(const Process *process) {
    return calculate_turnaround_time(process) - process->burst_time;
}

void simulate_mlfq(Process *processes, int count, int boost_interval) {
    Queue queues[QUEUE_LEVELS];
    int time = 0;
    int completed = 0;
    int next_arrival = 0;
    int current_process = -1;
    int queue_level;

    for (queue_level = 0; queue_level < QUEUE_LEVELS; queue_level++) {
        if (!init_queue(&queues[queue_level], count)) {
            fprintf(stderr, "No fue posible reservar memoria para las colas.\n");
            while (queue_level > 0) {
                queue_level--;
                destroy_queue(&queues[queue_level]);
            }
            return;
        }
    }

    while (completed < count) {
        while (next_arrival < count && processes[next_arrival].arrival_time == time) {
            processes[next_arrival].current_queue = 0;
            processes[next_arrival].slice_remaining = get_quantum_for_queue(0);
            enqueue(&queues[0], next_arrival);
            next_arrival++;
        }

        if (boost_interval > 0 && time > 0 && time % boost_interval == 0) {
            if (current_process != -1) {
                processes[current_process].current_queue = 0;
                processes[current_process].slice_remaining = get_quantum_for_queue(0);
                enqueue(&queues[0], current_process);
                current_process = -1;
            }

            boost_waiting_processes(queues, processes, count);
        }

        if (current_process != -1) {
            int highest_ready = find_highest_non_empty_queue(queues);

            if (highest_ready != -1 && highest_ready < processes[current_process].current_queue) {
                enqueue(&queues[processes[current_process].current_queue], current_process);
                current_process = -1;
            }
        }

        if (current_process == -1) {
            int selected_queue = find_highest_non_empty_queue(queues);

            if (selected_queue != -1) {
                dequeue(&queues[selected_queue], &current_process);

                if (processes[current_process].start_time == -1) {
                    processes[current_process].start_time = time;
                }

                if (processes[current_process].first_response_time == -1) {
                    processes[current_process].first_response_time = time;
                }
            }
        }

        if (current_process == -1) {
            time++;
            continue;
        }

        processes[current_process].remaining_time--;
        processes[current_process].slice_remaining--;
        time++;

        if (processes[current_process].remaining_time == 0) {
            processes[current_process].finish_time = time;
            completed++;
            current_process = -1;
            continue;
        }

        if (processes[current_process].slice_remaining == 0) {
            if (processes[current_process].current_queue < QUEUE_LEVELS - 1) {
                processes[current_process].current_queue++;
            }

            processes[current_process].slice_remaining =
                get_quantum_for_queue(processes[current_process].current_queue);

            enqueue(&queues[processes[current_process].current_queue], current_process);
            current_process = -1;
        }
    }

    for (queue_level = 0; queue_level < QUEUE_LEVELS; queue_level++) {
        destroy_queue(&queues[queue_level]);
    }
}

void print_results_table(const Process *processes, int count) {
    int index;

    printf("\nResultados de la simulacion\n");
    printf("%-6s %-8s %-6s %-6s %-7s %-9s %-11s %-8s\n",
           "PID", "Arrival", "Burst", "Start", "Finish", "Response", "Turnaround", "Waiting");

    for (index = 0; index < count; index++) {
        printf("%-6s %-8d %-6d %-6d %-7d %-9d %-11d %-8d\n",
               processes[index].pid,
               processes[index].arrival_time,
               processes[index].burst_time,
               processes[index].start_time,
               processes[index].finish_time,
               calculate_response_time(&processes[index]),
               calculate_turnaround_time(&processes[index]),
               calculate_waiting_time(&processes[index]));
    }
}

int export_results_csv(const char *filename, const Process *processes, int count) {
    FILE *file = fopen(filename, "w");
    int index;

    if (file == NULL) {
        return 0;
    }

    fprintf(file, "PID,Arrival,Burst,Start,Finish,Response,Turnaround,Waiting\n");

    for (index = 0; index < count; index++) {
        fprintf(file, "%s,%d,%d,%d,%d,%d,%d,%d\n",
                processes[index].pid,
                processes[index].arrival_time,
                processes[index].burst_time,
                processes[index].start_time,
                processes[index].finish_time,
                calculate_response_time(&processes[index]),
                calculate_turnaround_time(&processes[index]),
                calculate_waiting_time(&processes[index]));
    }

    fclose(file);
    return 1;
}
