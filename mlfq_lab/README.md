# Simulador de MLFQ

Este proyecto implementa un scheduler **Multi-Level Feedback Queue (MLFQ)** con:

- 3 colas de prioridad
- Round Robin dentro de cada cola
- Democión cuando un proceso consume todo su quantum
- Priority boost periódico
- Cálculo de métricas por proceso
- Exportación de resultados al archivo `results.csv`


## Política del scheduler

- **Q0**: prioridad alta, quantum = 2
- **Q1**: prioridad media, quantum = 4
- **Q2**: prioridad baja, quantum = 8
- Siempre se ejecuta la cola de mayor prioridad disponible
- Si un proceso agota su quantum y no termina, baja de nivel
- Si termina antes de agotar su quantum, no baja
- Cada cierto número de ciclos ocurre un **priority boost** y todos vuelven a Q0

## Métricas calculadas

Para cada proceso se calculan:

- **Response time** = first_response_time - arrival_time
- **Turnaround time** = finish_time - arrival_time
- **Waiting time** = turnaround_time - burst_time

## Formato del archivo de entrada

El archivo `assets/processes.txt` usa este formato:

```text
PID arrival burst
```

Ejemplo:

```text
P1 0 8
P2 1 4
P3 2 9
P4 3 5
```

Las líneas que comiencen con `#` son comentarios.

## Cómo compilar

```bash
make
```

## Cómo ejecutar

Con el archivo por defecto:

```bash
make run
```

O manualmente:

```bash
./scheduler_sim
```

## Salida

El programa muestra una tabla en consola y además crea un archivo llamado:

```text
results.csv
```
