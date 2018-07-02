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
/*#define MSJ_REGISTROS "REGISTROS:"
#define MSJ_ACC "acumulador:"
#define MSJ_CONTADOR "program counter:"
#define MSJ_INSTRUCCION "instruccion:"
#define MSJ_OPCODE "opcode:"
#define MSJ_OPERANDO "operando:"
#define MSJ_MEMORIA "MEMORIA:"*/
#define MASK_CHAR 0xff00
#define SHIFT_CHAR 8
#define CARAC_IMPRIMIBLE '.'
#define CANT_COLS 7

status_t imprimir_dump(lista_t lista, fmt_t formato);
status_t imprimir (const simpletron_t * simpletron, FILE * f_output);
status_t imprimir_registros (const struct simpletron * simpletron, FILE * f_output);
status_t imprimir_memoria (const simpletron_t * simpletron, FILE * f_output);
void imprimir_ascii(const vector_t * vector_memoria, int i, FILE * f_output);
status_t imprimir_bin(const simpletron_t *simpletron, FILE *f_output);


#endif
