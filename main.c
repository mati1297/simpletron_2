#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "./lista/tda_lista.h"
#include "lectura.h"
#include "simpletron.h"
#include "dump.h"

int main (int argc, char * argv []) {
	status_t st;
	status_simpletron st_smp;
	retval_t st_lista;
	params_t parametros;
	FILE * f_out;
	lista_t lista;
	
	if ((st = procesar_argumentos (argc, argv, &parametros, &cant_archivos)) != ST_OK) {
		if (st != ST_HELP) {
			imprimir_error (st);
			return EXIT_FAILURE;
		}
		if ((st = imprimir_ayuda (stdout)) != ST_OK) {
			imprimir_error (st);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	
	if ((st = crear_cargar_lista_simpletron (lista, &parametros)) != ST_OK) {
		imprimir_error (st);
		return EXIT_FAILURE;
	}
	
	if ((st_smp = ejecutar_simpletron (lista) {
		imprimir_error_simpletron (st_smp);
		return EXIT_FAILURE;
	}
	
	if ((st = imprimir_dump (lista, parametros->fmt_salida)) != ST_OK) {
		imprimir_error (st);
		return EXIT_FAILURE;
	}
	
	liberar_vector_archivos (parametros->vector_datos_archivos);
	if ((st_lista = LISTA_destruir (&lista, &simpletron_borrar)) != RV_SUCCESS) {
		imprimir_error_lista (st_lista);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

