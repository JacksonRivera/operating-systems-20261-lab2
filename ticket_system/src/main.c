#include "ticket/ticket.h"
#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  const char *assets_dir = "assets";

  // crear assets si no existe
  if (utils_ensure_dir(assets_dir) != 0) {
    fprintf(stderr, "Error: no se pudo preparar la carpeta assets.\n");
    return 1;
  }

  char buffer[256];

  // 1) Identificación 
  long id = 0;
  while (1) {
    if (utils_read_line("Identificacion (numerica): ", buffer, sizeof(buffer)) != 0) {
      fprintf(stderr, "Error leyendo la identificacion.\n");
      return 1;
    }

    if (buffer[0] == '\0') {
      printf("No se permite un valor vacio.\n");
      continue;
    }

    if (utils_parse_long(buffer, &id) != 0) {
      printf("Identificacion invalida. Debe ser numerica.\n");
      continue;
    }

    break;
  }

  // 2) Email
  char email[256];
  while (1) {
    if (utils_read_line("Correo electronico: ", email, sizeof(email)) != 0) {
      fprintf(stderr, "Error leyendo el correo.\n");
      return 1;
    }

    if (email[0] == '\0') {
      printf("No se permite un valor vacio.\n");
      continue;
    }

    if (!utils_validate_email(email)) {
      printf("Correo invalido. Debe contener '@'.\n");
      continue;
    }

    break;
  }

  // 3) Tipo de reclamación
  char claim_type[256];
  while (1) {
    if (utils_read_line("Tipo de reclamacion: ", claim_type, sizeof(claim_type)) != 0) {
      fprintf(stderr, "Error leyendo el tipo de reclamacion.\n");
      return 1;
    }

    if (claim_type[0] == '\0') {
      printf("No se permite un valor vacio.\n");
      continue;
    }

    break;
  }

  // Semilla de rand 
  srand((unsigned)time(NULL));

  // 4) Generar radicado único
  long long radicado = ticket_generate_radicado();

  // 5) Crear ticket con memoria dinámica
  Ticket *t = ticket_create(id, email, claim_type, radicado);
  if (!t) {
    fprintf(stderr, "Error: no se pudo asignar memoria para el ticket.\n");
    return 1;
  }

  // 6) Crear archivo en assets y escribir contenido
  if (ticket_write_to_file(t, assets_dir) != 0) {
    fprintf(stderr, "Error: no se pudo crear/escribir el archivo del ticket.\n");
    ticket_destroy(t);
    return 1;
  }

  // 7) Mostrar radicado al final
  printf("\nTicket registrado con exito.\nRadicado generado: %lld\n", radicado);

  // 8) Liberar memoria
  ticket_destroy(t);

  return 0;
}