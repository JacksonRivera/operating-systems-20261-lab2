# Ticket System (Reto #2)

Programa en C que registra tickets de reclamación por consola, genera un radicado único y crea un archivo en `assets/` con la información del ticket.

## Estructura del proyecto


ticket_system/
├── src/
│ ├── main.c
│ ├── ticket/ticket.c
│ └── utils/utils.c
├── include/
│ ├── ticket/ticket.h
│ └── utils/utils.h
├── assets/
├── Makefile
└── README.md


- `main.c`: flujo principal (captura de datos, validación, creación del ticket y confirmación del radicado).
- `ticket.c / ticket.h`: lógica de negocio del ticket (estructura, generación de radicado, escritura a archivo).
- `utils.c / utils.h`: funciones auxiliares (lectura segura con `fgets`, trims, validaciones, parse numérico, etc).

## Cómo compilar

Requisitos: `gcc`

```bash
make
