#include <stdio.h>
#include "error.h"
#include "idioma.h"
#include "tda_lista.h"
#include "simpletron.h"

void imprimir_error (status_t st) {
	
	static char * error [MAX_ERROR_STR]= {
		MSJ_NO_ERROR,
		MSJ_ERROR_PUNTERO_NULO,
		MSJ_ERROR_CANTIDAD_DE_ARGS,
		MSJ_ERROR_MEMORIA_INVALIDA,
		MSJ_ERROR_ENTRADA_INVALIDA,
		MSJ_ERROR_ARGUMENTO_INVALIDO,
		MSJ_ERROR_INSTRUCCION_INVALIDA,
		MSJ_ERROR_LECTURA_ARCHIVO,
		MSJ_ERROR_CANTIDAD_PALABRAS_INVALIDA,
		MSJ_ERROR_CANTIDAD_ARCHIVOS,
		MSJ_ERROR_LISTA,
		MSJ_ERROR_MEMORIA_INSUFICIENTE
	};
	
	if(st != ST_OK)
		fprintf (stderr, "%s: %s\n", MSJ_PREFIJO_ERROR, error [st]);
} 

void imprimir_error_simpletron (status_simpletron st) {
	static char * error [MAX_ERROR_STR] = {
		MSJ_NO_ERROR,
		MSJ_ERROR_PUNTERO_NULO,
		MSJ_ERROR_SIMPLETRON
	};
	
	if(st != ST_SMP_OK)
		fprintf (stderr, "%s: %s\n", MSJ_PREFIJO_ERROR_SIMPLETRON, error [st]);
}

void imprimir_error_lista (retval_t st) {
	static char * error [MAX_ERROR_STR] = {
		MSJ_NO_ERROR,
		MSJ_ERROR_PUNTERO_NULO,
		MSJ_ERROR_MEMORIA_INSUFICIENTE,
		MSJ_ERROR,
		MSJ_ERROR_NO_IMPLEMENTADO,
		MSJ_ERROR_ARGUMENTO_INVALIDO
	};

	if(st != RV_SUCCESS)
		fprintf (stderr, "%s: %s\n", MSJ_PREFIJO_ERROR_LISTA, error [st]);
}
