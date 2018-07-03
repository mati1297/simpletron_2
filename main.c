#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "tda_lista.h"
#include "lectura.h"
#include "simpletron.h"
#include "dump.h"
#include "procesar_arg.h"

int main (int argc, const char * argv []) {
	status_t st;
	status_simpletron st_smp;
	retval_t st_lista;
	params_t parametros;
	lista_t lista;
	
	if ((st = procesar_argumentos (argc, argv, &parametros)) != ST_OK) {
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
	
	if ((st = crear_cargar_lista_simpletron (&lista, &parametros)) != ST_OK) {
		imprimir_error (st);
		liberar_vector_archivos(parametros.vector_datos_archivos);
		return EXIT_FAILURE;
	}

	/*si falla es que fallo el simpletron pero igual se imprime el dump
	 * aunque no se si que lo hagamos asi esta bien porque el siguiente programa
	 * ya no se llega a ejecutar, MIRA ESTO SANTI*/
	if ((st_smp =simpletron_ejecutar_lista (&lista)) != ST_SMP_OK) {
		imprimir_error_simpletron (st_smp);
	}

	if ((st = imprimir_dump (lista, parametros.fmt_salida)) != ST_OK) {
		liberar_vector_archivos(parametros.vector_datos_archivos);
		LISTA_destruir(&lista, &simpletron_borrar);
		imprimir_error (st);
		return EXIT_FAILURE;
	}
	/*En ningun lugar lo validamos aca tampoco se si da, no deberia fallar nunca
	 * a no ser que se le pase un puntero nulo*/
	if ((st_lista = LISTA_destruir (&lista, &simpletron_borrar)) != RV_SUCCESS) {
		liberar_vector_archivos(parametros.vector_datos_archivos);
		imprimir_error_lista (st_lista);
		return EXIT_FAILURE;
	}
	liberar_vector_archivos(parametros.vector_datos_archivos);
	return EXIT_SUCCESS;
}

