#ifndef DUMP__H
#define DUMP__H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "error.h"
#include "simpletron.h"
#include "procesar_arg.h"
#include "vector.h"
#include "idioma.h"

#define NOMBRE_ARCHIVO_STANDARD_TXT "output.txt"
#define NOMBRE_ARCHIVO_STANDARD_BIN "output.bin"
#define MASK_CHAR 0xff00
#define SHIFT_CHAR 8
#define CARAC_NO_IMPRIMIBLE '.'
#define CANT_COLS 8

status_t imprimir_dump(lista_t lista, fmt_t formato);
status_t imprimir (const simpletron_t * simpletron, FILE * f_output);
status_t imprimir_registros (const struct simpletron * simpletron, FILE * f_output);
status_t imprimir_memoria (const simpletron_t * simpletron, FILE * f_output);
void imprimir_ascii(const vector_t * vector_memoria, size_t inicio, size_t final, FILE * f_output);
status_t imprimir_bin(const simpletron_t *simpletron, FILE *f_output);


#endif
