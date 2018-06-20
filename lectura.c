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

status_t leer_guardar_archivo (archivos * archivo, vector_t * vector) {
	FILE * file;
	char buffer [MAX_LARGO], *endp;
	int aux;
	palabra_t dato;
	
	if(!archivo)
		return ST_ERROR_PUNTERO_NULO;
		
	if(!(file = fopen(archivo->nombre_archivo, "r")))
		return ST_ERROR_LECTURA_ARCHIVO;
		
	if(archivo->fmt_entrada == FMT_TXT) {
		while(fgets(buffer, MAX_LARGO, file)) {
			if(!recortar_espacios(buffer))
				continue;
			cortar_delimitador(buffer, DELIMITADOR_COMENTARIO);
			aux = (palabra_t) strtol(buffer, &endp, 10);
			
			/*LO HAGO PARTIENDOLO*/
			if(aux / DIVISOR > MAX_OPERANDO)
				return /*error??*/;
			palabra_guardar_opcode(&dato, aux / DIVISOR);
			if(aux % DIVISOR > MAX_OPCODE)
				return /*idem arriba*/;
			palabra_guardar_opcode(&dato, aux % DIVISOR);
		}
	}
		/*Puede hacer falta hacer un fgets mas para llegar a eof*/
	else {
		while(fread(&dato, sizeof(int), 1, file)) {
			/*LO HAGO PARTIENDOLO*/
			if(aux / DIVISOR > MAX_OPERANDO)
				return /*error??*/;
			palabra_guardar_opcode(&dato, aux / DIVISOR);
			if(aux % DIVISOR > MAX_OPCODE)
				return /*idem arriba*/;
			palabra_guardar_opcode(&dato, aux % DIVISOR);
		}
	}
	
	if(!feof(file))
			return ST_ERROR_LECTURA_ARCHIVO;
	if(ferror(file))
			return ST_ERROR_LECTURA_ARCHIVO;
}


