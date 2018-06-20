#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lectura.h"
#include "simpletron.h"
#include "procesar_arg.c"
#include "vector.h"
#include "herramientas.h"
#include "error.h"

/*status_t cargar_lista_archivos*/

/*Pueden faltar cosas, revisar, creo que esta todo
 * ESTA FUCION SE VA A LLAMAR POR CADA ARCHIVO QUE SE LEA*/
status_t leer_guardar_archivo (archivos * archivo, vector_t * vector) {
	FILE * file;
	char buffer [MAX_LARGO], *endp;
	int aux;
	size_t i;
	palabra_t dato;
	status_vector st_vec;
	
	if(!archivo)
		return ST_ERROR_PUNTERO_NULO;
		
	if(!(file = fopen(archivo->nombre_archivo, "r")))
		return ST_ERROR_LECTURA_ARCHIVO;
		
	if(archivo->fmt_entrada == FMT_TXT) {
		for(i = 0; fgets(buffer, MAX_LARGO, file); i++) {
			if(!recortar_espacios(buffer))
				continue;
			cortar_delimitador(buffer, DELIMITADOR_COMENTARIO);
			aux = (int) strtol(buffer, &endp, 10);
			if(!*endp)
				return ST_ERROR_INSTRUCCION_INVALIDA;
			
			/*LO HAGO PARTIENDOLO*/
			/*falta validar, no se QUE hay que validar*/
			dato = palabra_dividir(aux);
			if((st_vec = vector_guardar(vector, i, &dato, &palabra_copiar, &palabra_destruir)) != ST_VEC_OK)
				return st_vec; 
			
		}
	}
		/*Puede hacer falta hacer un fgets mas para llegar a eof*/
	else {
		for(i = 0; fread(&aux, sizeof(int), 1, file); i++) {
			/*LO HAGO PARTIENDOLO*/
			/*falta validar, no se QUE hay que validar*/
			dato = palabra_dividir(aux);
			vector_guardar(vector, i, &dato, &palabra_copiar, &palabra_destruir);
		}
	}
	
	if(!feof(file))
			return ST_ERROR_LECTURA_ARCHIVO;
	if(ferror(file))
			return ST_ERROR_LECTURA_ARCHIVO;
	fclose(file);
	return ST_OK;
}


