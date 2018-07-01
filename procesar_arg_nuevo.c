#include "procesar_arg.h"

status_t cargar_argumentos_por_omision (params_t * parametros) {
	if (parametros == NULL)
		return ST_ERROR_PUNTERO_NULO;
		
	parametros->cant_memoria = MEMORIA_POR_OMISION;
	parametros->fmt_salida = FMT_TXT;
	
	return ST_OK;
}

status_t procesar_argumentos (const int argc, const char * argv [], params_t * parametros) {
	char * endp;
	
	const char * arg_validos [] = {
		TXT_ARG_MEMORIA,
		TXT_ARG_FMT_SALIDA,
		TXT_ARG_AYUDA
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
			}
			switch (j) {
				case ARG_MEMORIA:
					i++;
					parametros->cant_memoria = strtol(argv[i], &endp, 10);
					if(*endp)
						return ST_ERROR_ARGUMENTO_INVALIDO;
					
			}
		}
	}
}
