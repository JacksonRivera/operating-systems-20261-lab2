#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"

static int compare_processes_by_arrival(const void *a, const void *b) {
    const Process *process_a = (const Process *)a;
    const Process *process_b = (const Process *)b;

    if (process_a->arrival_time != process_b->arrival_time) {
        return process_a->arrival_time - process_b->arrival_time;
    }

    return strcmp(process_a->pid, process_b->pid);
}

int load_processes_from_file(const char *filename, Process **processes_out) {
    FILE *file = fopen(filename, "r");
    Process *processes = NULL;
    char line[256];
    int capacity = 4;
    int count = 0;

    if (file == NULL) {
        return -1;
    }

    processes = (Process *)malloc(sizeof(Process) * capacity);
    if (processes == NULL) {
        fclose(file);
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char pid[PID_MAX_LENGTH];
        int arrival_time;
        int burst_time;

        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        if (sscanf(line, "%31s %d %d", pid, &arrival_time, &burst_time) != 3) {
            continue;
        }

        if (count == capacity) {
            Process *new_block;

            capacity *= 2;
            new_block = (Process *)realloc(processes, sizeof(Process) * capacity);
            if (new_block == NULL) {
                free(processes);
                fclose(file);
                return -1;
            }

            processes = new_block;
        }

        init_process(&processes[count], pid, arrival_time, burst_time);
        count++;
    }

    fclose(file);

    qsort(processes, count, sizeof(Process), compare_processes_by_arrival);

    *processes_out = processes;
    return count;
}

void free_processes(Process *processes) {
    free(processes);
}

void print_input_summary(const Process *processes, int count) {
    int index;

    printf("Procesos cargados desde el archivo\n");
    printf("%-6s %-8s %-6s\n", "PID", "Arrival", "Burst");

    for (index = 0; index < count; index++) {
        printf("%-6s %-8d %-6d\n",
               processes[index].pid,
               processes[index].arrival_time,
               processes[index].burst_time);
    }
}
