#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lectura.h"
#include "vector.h"
#include "simpletron.h"
#include "procesar_arg.c"
#include "herramientas.h"
#include "error.h"

/*status_t cargar_lista_archivos*/

/*Pueden faltar cosas, revisar, creo que esta todo
 * ESTA FUCION SE VA A LLAMAR POR CADA ARCHIVO QUE SE LEA*/
status_t leer_guardar_archivo (archivo_t * archivo, vector_t * vector, size_t memoria_pedida) {
	FILE * file;
	char buffer [MAX_LARGO], *endp;
	int aux;
	size_t i;
	palabra_t dato;
	status_vector st_vec;
	
	if (!archivo)
		return ST_ERROR_PUNTERO_NULO;
		
	if (archivo->fmt_entrada == FMT_TXT) {
		if (!(file = fopen(archivo->nombre_archivo, "rt")))
			return ST_ERROR_LECTURA_ARCHIVO;
		for (i = 0; fgets (buffer, MAX_LARGO, file); i++) {
			if (i > memoria_pedida) {
				fclose (file);
				return ST_ERROR_CANTIDAD_PALABRAS_INVALIDA;
			}
			if (!recortar_espacios (buffer))
				continue;
			cortar_delimitador (buffer, DELIMITADOR_COMENTARIO);
			aux = (int) strtol (buffer, &endp, 10);
			
			if (*endp) {
				fclose (file);
				return ST_ERROR_INSTRUCCION_INVALIDA;
			}
			
			/*LO HAGO PARTIENDOLO*/
			/*falta validar, no se QUE hay que validar*/
			dato = palabra_dividir (aux);
			if ((st_vec = vector_guardar (vector, i, &dato, &palabra_copiar, &palabra_destruir)) != ST_VEC_OK) {
				fclose (file);
				return st_vec;
			} 
			
		}
	}
		/*Puede hacer falta hacer un fgets mas para llegar a eof*/
	else {
		if (!(file = fopen(archivo->nombre_archivo, "rb")))
		return ST_ERROR_LECTURA_ARCHIVO;
		for (i = 0; fread (&dato, sizeof (palabra_t), 1, file) != 1; i++) {
			/*falta validar, no se QUE hay que validar*/
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
	fclose (file);
	return ST_OK;
}


