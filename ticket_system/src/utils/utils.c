#include "utils/utils.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <direct.h>
  #define MKDIR(path) _mkdir(path)
#else
  #include <sys/stat.h>
  #include <sys/types.h>
  #define MKDIR(path) mkdir(path, 0755)
#endif

int utils_read_line(const char *prompt, char *buffer, size_t size) {
  if (!buffer || size == 0) return -1;

  if (prompt) {
    fputs(prompt, stdout);
    fflush(stdout);
  }

  if (!fgets(buffer, (int)size, stdin)) {
    // Error/EOF en lectura
    return -1;
  }

  // Quitar salto de línea si existe
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }

  utils_trim(buffer);
  return 0;
}

void utils_trim(char *s) {
  if (!s) return;

  // Trim izquierda
  size_t start = 0;
  while (s[start] != '\0' && isspace((unsigned char)s[start])) start++;

  if (start > 0) {
    memmove(s, s + start, strlen(s + start) + 1);
  }

  // Trim derecha
  size_t end = strlen(s);
  while (end > 0 && isspace((unsigned char)s[end - 1])) end--;
  s[end] = '\0';
}

int utils_parse_long(const char *s, long *out) {
  if (!s || !out) return -1;

  errno = 0;
  char *endptr = NULL;
  long val = strtol(s, &endptr, 10);

  if (errno != 0) return -1;
  if (endptr == s) return -1;       // no convirtió nada
  if (*endptr != '\0') return -1;   // caracteres sobrantes

  *out = val;
  return 0;
}

int utils_validate_email(const char *s) {
  if (!s || s[0] == '\0') return 0;
  return strchr(s, '@') != NULL;
}

int utils_ensure_dir(const char *path) {
  if (!path || path[0] == '\0') return -1;

  int rc = MKDIR(path);
  if (rc == 0) return 0;

#ifndef _WIN32
  return 0;
#else
  return 0;
#endif
}