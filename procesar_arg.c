#include <stdlib.h>
#include "procesar_arg.h"
#include "error.h"
#include <string.h>

status_t cargar_argumentos_por_omision (params_t * parametros) {
	if (parametros == NULL)
		return ST_ERROR_PUNTERO_NULO;
		
	parametros->cant_memoria = MEMORIA_POR_OMISION;
	parametros->fmt_salida = FMT_TXT;
	
	return ST_OK;
}

status_t procesar_argumentos (const int argc, const char * argv [], params_t * parametros) {
	status_t st;
	size_t aux;
	char * endp;
	size_t i, j;
	const char * arg_validos [] = {
		TXT_ARG_MEMORIA,
		TXT_ARG_FMT_SALIDA,
		TXT_ARG_AYUDA
	};
	const char * arg_validos_extendidos[] = {
		TXT_ARG_MEMORIA_EXTENDIDO,
		TXT_ARG_FMT_SALIDA_EXTENDIDO,
		TXT_ARG_AYUDA_EXTENDIDO
	};
	if(!parametros || !argv)
		return ST_ERROR_PUNTERO_NULO;
	/*al menos se debe ingresar el ejecutable y un archivo o el caracter indicador de stdin*/
	if (MIN_ARGC > argc) {
		return ST_ERROR_CANTIDAD_ARGUMENTOS;
	}
	if((st = cargar_argumentos_por_omision(parametros)) != ST_OK){
		return st;
	}
	/*si solo hay un argumento, se asume que es un archivo*/
	if (argc != MIN_ARGC) {
		/* Se empieza en 1 para omitir el argumento que contiene la linea de ejecucion */
		for(i = 1; i < MAX_MODIFICADORES; i++) {
			for (j = 0; j < ARG_INVALIDO; j++) {
				/* Si las cadenas son iguales se procede al switch, de otra forma se sigue iterando */
				if(!strcmp (argv[i], arg_validos[j]))
					break;
				if(!strcmp (argv[i], arg_validos_extendidos[j]))
					break;
			}
			switch (j) {
				case ARG_MEMORIA:
					i++;
					aux = (size_t) strtol(argv[i], &endp, 10);
					if(*endp)
						return ST_ERROR_ARGUMENTO_INVALIDO;
					if (aux < 1)
						return ST_ERROR_CANTIDAD_DE_MEMORIA_INVALIDA;
					parametros->cant_memoria = aux;
					break;
				case ARG_FMT_SALIDA:
					i++;
					if (!strcmp (argv[i], TXT_INDICADOR_BINARIO))
						parametros->fmt_salida = FMT_BIN;
					else if (!strcmp (argv[i], TXT_INDICADOR_TEXTO))
						parametros->fmt_salida = FMT_TXT;
					else
						return ST_ERROR_ARGUMENTO_INVALIDO;
					break;
				case ARG_AYUDA:
					return ST_HELP;
					break;
			}
			/*si el argumento es invalido asume que ya es un archivo, por lo tanto salta a la otra parte*/
			if(j == ARG_INVALIDO)
				break;
		}
		
		
			if((parametros->cant_archivos = argc - i) == 0)
				return ST_ERROR_CANTIDAD_ARCHIVOS;
			if(!(parametros-> vector_datos_archivos = (archivo_t *) calloc (parametros->cant_archivos, sizeof(archivo_t))))
				return ST_ERROR_MEMORIA_INSUFICIENTE;
		
		/*Comienza la lectura de los nombres de archivos*/
		for(; i < argc; i++) {
			
			
			if(!strncmp(argv[i], INDICADOR_BIN, strlen(INDICADOR_BIN)))
				parametros->vector_datos_archivos[i].fmt_entrada = FMT_BIN;
			else if(!strncmp(argv[i], INDICADOR_TXT, strlen(INDICADOR_TXT))) {
				parametros->vector_datos_archivos[i].fmt_entrada = FMT_TXT;
				parametros->vector_datos_archivos[i].nombre_archivo = argv[i] + POSICION_COMIENZO_ARCHIVO;
			}
			else if(!(strcmp(argv[i], INDICADOR_STDIN))) {
				if(parametros -> cant_archivos > 1) {
					liberar_vector_archivos(parametros->vector_datos_archivos);
					return ST_ERROR_ARGUMENTO_INVALIDO;
				}
				parametros->vector_datos_archivos->fmt_entrada = FMT_TXT;
				parametros->vector_datos_archivos->nombre_archivo = argv[i];
			}
			else {
				parametros->vector_datos_archivos->nombre_archivo = argv[i];
				parametros->vector_datos_archivos->fmt_entrada = FMT_TXT;
			}
		}
	}
	return ST_OK;
}


void liberar_vector_archivos (archivo_t * archivos) {
	if (archivos)
		free(archivos);
}
