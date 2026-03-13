#ifndef UTILS_H
#define UTILS_H

#include "scheduler/scheduler.h"

int load_processes_from_file(const char *filename, Process **processes_out);
void free_processes(Process *processes);
void print_input_summary(const Process *processes, int count);

#endif
