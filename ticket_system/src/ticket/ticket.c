#include "ticket/ticket.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

static char *copy_string(const char *src) {
  if (!src) return NULL;

  size_t len = strlen(src);
  char *dst = (char *)malloc(len + 1);
  if (!dst) return NULL;

  memcpy(dst, src, len + 1);
  return dst;
}

long long ticket_generate_radicado(void) {
  // Combinación de time(NULL) + rand() 
  long long now = (long long)time(NULL);   
  long long r = (long long)(rand() % 100000);
  return now * 100000LL + r;
}

Ticket *ticket_create(long id, const char *email, const char *claim_type, long long radicado) {
  Ticket *t = (Ticket *)malloc(sizeof(Ticket));
  if (!t) return NULL;

  t->id = id;
  t->radicado = radicado;

  t->email = copy_string(email);
  if (!t->email) {
    free(t);
    return NULL;
  }

  t->claim_type = copy_string(claim_type);
  if (!t->claim_type) {
    free(t->email);
    free(t);
    return NULL;
  }

  return t;
}

void ticket_destroy(Ticket *t) {
  if (!t) return;
  free(t->email);
  free(t->claim_type);
  free(t);
}

int ticket_write_to_file(const Ticket *t, const char *assets_dir) {
  if (!t || !assets_dir) return -1;

  char path[512];
  int n = snprintf(path, sizeof(path), "%s/ticket_%lld.txt", assets_dir, t->radicado);
  if (n < 0 || (size_t)n >= sizeof(path)) return -1;

  FILE *fp = fopen(path, "w");
  if (!fp) return -1; // Validar fopen != NULL :contentReference[oaicite:3]{index=3}

  // Contenido del archivo 
  fprintf(fp, "Radicado: %lld\n", t->radicado);
  fprintf(fp, "Identificacion: %ld\n", t->id);
  fprintf(fp, "Correo: %s\n", t->email);
  fprintf(fp, "Tipo de reclamacion: %s\n", t->claim_type);

  fclose(fp);
  return 0;
}