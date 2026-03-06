#ifndef TICKET_H
#define TICKET_H

#include <stdio.h>

typedef struct Ticket {
  long id;               // Identificación
  char *email;           // Correo 
  char *claim_type;      // Tipo de reclamación 
  long long radicado;    // Número de radicado
} Ticket;

long long ticket_generate_radicado(void);

Ticket *ticket_create(long id, const char *email, const char *claim_type, long long radicado);
void ticket_destroy(Ticket *t);

int ticket_write_to_file(const Ticket *t, const char *assets_dir);

#endif