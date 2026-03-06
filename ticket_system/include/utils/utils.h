#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

int utils_read_line(const char *prompt, char *buffer, size_t size);

void utils_trim(char *s);

int utils_parse_long(const char *s, long *out);

int utils_validate_email(const char *s);

int utils_ensure_dir(const char *path);

#endif