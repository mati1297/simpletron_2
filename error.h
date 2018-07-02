#ifndef ERROR__H
#define ERROR__H 1

#include "tda_lista.h"
#include "simpletron.h"
#include "idioma.h"

#define MAX_ERROR_STR 150

typedef enum {
	ST_OK,
	ST_ERROR_PUNTERO_NULO,
	ST_ERROR_CANTIDAD_ARGUMENTOS,
	ST_ERROR_MEMORIA_INVALIDA,
	ST_ERROR_ENTRADA_INVALIDA,
	ST_ERROR_ARGUMENTO_INVALIDO,
	ST_ERROR_INSTRUCCION_INVALIDA,
	ST_ERROR_LECTURA_ARCHIVO,
	ST_ERROR_CANTIDAD_PALABRAS_INVALIDA,
	ST_ERROR_CANTIDAD_ARCHIVOS,
	ST_ERROR_MEMORIA_INSUFICIENTE,
	ST_HELP
} status_t;

void imprimir_error (status_t);
void imprimir_error_lista (retval_t);
void imprimir_error_simpletron (status_simpletron);

#endif
