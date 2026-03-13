#include <stdio.h>
#include <stdlib.h>

#include "scheduler/scheduler.h"
#include "utils/utils.h"

int main(int argc, char *argv[]) {
    const char *input_file = "assets/processes.txt";
    const char *output_file = "results.csv";
    int boost_interval = 20;
    Process *processes = NULL;
    int process_count;

    if (argc >= 2) {
        input_file = argv[1];
    }

    if (argc >= 3) {
        boost_interval = atoi(argv[2]);
    }

    process_count = load_processes_from_file(input_file, &processes);
    if (process_count <= 0) {
        fprintf(stderr, "No fue posible cargar procesos desde %s\n", input_file);
        return 1;
    }

    print_input_summary(processes, process_count);

    printf("\nConfiguracion del scheduler\n");
    printf("Q0 - quantum 2\n");
    printf("Q1 - quantum 4\n");
    printf("Q2 - quantum 8\n");
    printf("Priority boost cada %d ciclos\n", boost_interval);

    simulate_mlfq(processes, process_count, boost_interval);
    print_results_table(processes, process_count);

    if (!export_results_csv(output_file, processes, process_count)) {
        fprintf(stderr, "No fue posible generar el archivo %s\n", output_file);
        free_processes(processes);
        return 1;
    }

    printf("\nArchivo generado correctamente: %s\n", output_file);

    free_processes(processes);
    return 0;
}
