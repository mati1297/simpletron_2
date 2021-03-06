#include <stdio.h>
#include <string.h>
#include "lectura.h"
#include "vector.h"
#include "simpletron.h"
#include "procesar_arg.h"
#include "herramientas.h"
#include "error.h"
#include "tda_lista.h"

status_t crear_cargar_lista_simpletron (lista_t * lista, params_t * parametros) {
	size_t i;
	status_t st;
	simpletron_t * simpletron;
	if(!parametros)
	LISTA_crear(lista);
	for (i = 0; i < parametros->cant_archivos; i++) {
		simpletron = simpletron_crear (parametros->cant_memoria);
		if((st = leer_guardar_archivo(&(parametros->vector_datos_archivos[i]), simpletron->vector, simpletron->cantidad_de_memoria)) != ST_OK) {
			simpletron_borrar(simpletron);
			return st;
		}
		if(LISTA_insertar_al_final(lista, simpletron) != RV_SUCCESS) {
			simpletron_borrar(simpletron);
			return ST_ERROR_LISTA;
		}
	}
	return ST_OK;
}

/* ESTA FUCION SE VA A LLAMAR POR CADA ARCHIVO QUE SE LEA*/
status_t leer_guardar_archivo (archivo_t * archivo, vector_t * vector, size_t memoria_pedida) {
	FILE * file;
	char buffer [MAX_LARGO], *endp;
	long aux;
	size_t i;
	palabra_t dato;
	if (!archivo)
		return ST_ERROR_PUNTERO_NULO;
	if (archivo->fmt_entrada == FMT_TXT) {
		if (!strcmp(archivo->nombre_archivo, INDICADOR_STDIN))
			file = stdin;
		else if (!(file = fopen(archivo->nombre_archivo, "rt"))) {
			return ST_ERROR_LECTURA_ARCHIVO;
		}
		for (i = 0; fgets (buffer, MAX_LARGO, file); i++) {
			if (i >= memoria_pedida) {
				fclose (file);
				return ST_ERROR_CANTIDAD_PALABRAS_INVALIDA;
			}
			cortar_delimitador (buffer, DELIMITADOR_COMENTARIO);
			if (!recortar_espacios (buffer))
				continue;
			aux = strtol (buffer, &endp, 10);
			if (*endp && *endp != '\n') {
				fclose (file);
				return ST_ERROR_INSTRUCCION_INVALIDA;
			}
			
			if(palabra_validar_generar(aux, &dato) != ST_SMP_OK)
				return ST_ERROR_PALABRA;
			if (vector_guardar (vector, i, &dato, &palabra_copiar, &palabra_destruir)) {
				fclose (file);
				return ST_ERROR_VECTOR;
			} 
			
		}
	}
		/*Ya se lee como palabra_t*/
	else {
		if (!(file = fopen(archivo->nombre_archivo, "rb"))) {
			return ST_ERROR_LECTURA_ARCHIVO;
		}
		for (i = 0; fread (&dato, sizeof (palabra_t), 1, file) != 1; i++) {
			if (i > memoria_pedida) {
				fclose (file);
				return ST_ERROR_CANTIDAD_PALABRAS_INVALIDA;
			}
			vector_guardar (vector, i, &dato, &palabra_copiar, &palabra_destruir);
			/* guardamos el dato como viene, es decir, asumimos que el dato en binario viene como para meterlo de una en simpletron */
		}
	}
	if (!feof (file)) {
		fclose (file);
		return ST_ERROR_LECTURA_ARCHIVO;
	}
	if (ferror (file)) {
		fclose (file);
		return ST_ERROR_LECTURA_ARCHIVO;
	}
	
	dato = PALABRA_VACIA;
	for(; i < memoria_pedida; i++)
	{
		vector_guardar(vector, i, &dato, &palabra_copiar, &palabra_destruir);
	}
	
	fclose (file);
	return ST_OK;
}


