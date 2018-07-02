#ifndef PROCESAR_ARG__H
#define PROCESAR_ARG__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define MIN_ARGC 2
#define MEMORIA_POR_OMISION 50
#define TXT_ARG_MEMORIA "-m"
#define TXT_ARG_FMT_SALIDA "-f"
#define TXT_ARG_AYUDA "-h"
#define TXT_ARG_AYUDA_EXTENDIDO "--help"
#define TXT_ARG_FMT_SALIDA_EXTENDIDO "--format"
#define TXT_ARG_MEMORIA_EXTENDIDO "--memory"
#define TXT_INDICADOR_BINARIO "bin"
#define TXT_INDICADOR_TEXTO "txt"
#define MAX_MODIFICADORES 2
#define POS_FMT 0
#define INDICADOR_STDIN "-"
#define INDICADOR_BIN "b:"
#define INDICADOR_TXT "t:"
#define DELIM ':'
#define MAX_STR 50
#define SIZE_ARCHIVO_OMISION 5
#define CANT_ARG_MODIF 2
#define POSICION_COMIENZO_ARCHIVO 2

typedef enum {
	ARG_MEMORIA = 0,
	ARG_FMT_SALIDA = 1,
	ARG_AYUDA = 2,
	ARG_INVALIDO = 3
} arg_t;

typedef enum {
	FMT_TXT = 0,
	FMT_BIN = 1
} fmt_t;

typedef struct archivo {
	fmt_t fmt_entrada;
	const char * nombre_archivo;
} archivo_t;

typedef struct parametros {
	size_t cant_memoria;
	fmt_t fmt_salida;
	int cant_archivos;
	archivo_t * vector_datos_archivos;
} params_t;

status_t procesar_argumentos(const int argc, const char * argv [], params_t * parametros);
status_t cargar_argumentos_por_omision (params_t * parametros);
void liberar_vector_archivos (archivo_t * archivos);

#endif
