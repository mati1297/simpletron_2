#include "procesar_arg.h"

/* Recibe la estructura de los parámetros por puntero y carga los
 * parámetros ingresados por el usuario. En caso no se haya ingresado
 * algún argumento quedará el valor predeterminado. Devuelve el estado
 * por el nombre para informar de haber algún error */
status_t cargar_argumentos_por_omision (params_t *parametros) {
	if (parametros == NULL)
		return ST_ERROR_PUNTERO_NULO;
		
	parametros->cant_memoria = MEMORIA_POR_OMISION;
	parametros->fmt_salida = FMT_TXT;
	
	return ST_OK;
}

/* Recibe los argumentos ingresados por el usuario en el vector de punteros
 * argv, un puntero a la estructura de parámetros donde se van a guardar
 * los parámetros ingresados por el usuario, y la cantidad de parámetros
 * ingresados para las validaciones y ciclos. Se asume que si se usa stdin
 * será en formato txt. Devuelve el estado por el nombre, informando así 
 * si hay algún error */
status_t procesar_argumentos (const int argc, const char * argv [], params_t * parametros, int * cant_archivos){
	int i, j, cant_modif = 0;															/*la cantidad de archivos despues podriamos hacer que salga de una estructura de simpletron
																					 * o algo asi */
	char * endp;
	const char * arg_validos [] = {
		TXT_ARG_MEMORIA,
		TXT_ARG_FMT_SALIDA,
		TXT_ARG_AYUDA
	};
	status_t st;		
	if(!argv || !parametros || !cant_archivos) {												 
		return ST_ERROR_PUNTERO_NULO;
	}
	/*al menos se debe ingresar el ejecutable y un archivo o el caracter indicador de stdin*/
	if (MIN_ARGC > argc) {
		return ST_ERROR_CANTIDAD_ARGUMENTOS;
	}
	if((st = cargar_argumentos_por_omision(parametros)) != ST_OK){
		return st;
	}
	if (argc != MIN_ARGC){
		/* Se empieza en 1 para omitir el argumento que contiene la linea de ejecucion */
		for (i = 1; i <= MAX_MODIFICADORES; i++) {
			for (j = 0; j < ARG_INVALIDO; j++) {
				/* Si las cadenas son iguales se procede al switch, de otra forma se sigue iterando */
				if(!strcmp (argv[i], arg_validos[j]))
					break;
			}
			switch (j) {
				
				/* Se guarda la cantidad de instrucciones a interpretar */
				case ARG_MEMORIA:
					i++;
					parametros->cant_memoria = strtol (argv[i], &endp, 10);
					if (*endp) 
						return ST_ERROR_ARGUMENTO_INVALIDO;

					if (parametros->cant_memoria <= 0)
						return ST_ERROR_CANTIDAD_DE_MEMORIA_INVALIDA;
					cant_modif = cant_modif + CANT_ARG_MODIF;
					break;
				/* Se guarda el formato de salida*/	
				case ARG_FMT_SALIDA:
					i++;
					if (!strcmp (argv[i], TXT_INDICADOR_BINARIO))
						parametros->fmt_salida = FMT_BIN;
					else if (!strcmp (argv[i], TXT_INDICADOR_TEXTO))
						parametros->fmt_salida = FMT_TXT;
					else
						return ST_ERROR_ARGUMENTO_INVALIDO;
					cant_modif = cant_modif + CANT_ARG_MODIF;
					break;

				/* Llama a la función de impresión de la ayuda */
				case ARG_AYUDA:
					return ST_HELP;
					
				default: break; 
			}
		}		/* fijate que cant_arg solo cuenta 1 por cada PAR de argumentos, y por eso lo multiplicas por dos */
				/* es realmente cant_arg? porque la cantidad posta son mas */
	}
	*cant_archivos = argc - cant_modif - 1;				/*de aca en mas todavia no lei*/
	/*en el caso en el que se ingrese que el archivo de entrada es "-" se asume que el nombre del archivo es stdin, es unico y el formato, txt*/
	if (argv[2*cant_arg + 1] [POS_FMT] == INDICADOR_STDIN) {
		parametros->vector_datos_archivos = (archivo_t *) calloc (1, sizeof(archivo_t));
		if (!(parametros->vector_datos_archivos))
			return ST_ERROR_MEMORIA_INVALIDA;
		parametros->vector_datos_archivos->fmt_entrada = FMT_TXT;
		parametros->vector_datos_archivos->nombre_archivo = (char *) malloc (sizeof (char) * SIZE_ARCHIVO_OMISION);
		if (!(parametros->vector_datos_archivos->nombre_archivo))
			return ST_ERROR_MEMORIA_INVALIDA;
		strcpy (parametros->vector_datos_archivos->nombre_archivo, ARCHIVO_POR_OMISION);
		return ST_OK;
	}
	/*en el caso en el que se ingresen los nombres de los archivos*/
	parametros->vector_datos_archivos = (archivo_t *) calloc (*cant_archivos, sizeof (archivo_t));
	if (!(parametros->vector_datos_archivos))
		return ST_ERROR_MEMORIA_INVALIDA;
	for (i = 2*cant_arg + 1; i < argc; i++){
		/*primero se ve si se indico el formato para el archivo de entrada correspondiente a la iteracion.
		 * De ser asi, se guarda el formato indicado y el nombre del archivo
		 * Sino se guarda el nombre ingresado y se asume que el formato es txt*/
		if (endp = strchr(argv[i], DELIM)) {
			if(argv[i] [POS_FMT] == INDICADOR_BIN)
				parametros->vector_datos_archivos[i].fmt_entrada = FMT_BIN;
			else if (argv[i] [POS_FMT] == INDICADOR_TXT) {
				parametros->vector_datos_archivos[i].fmt_entrada = FMT_TXT;
			}
			else
				return ST_ERROR_ARGUMENTO_INVALIDO;
			parametros->vector_datos_archivos[i].nombre_archivo = endp + 1;
		}
		else {
			parametros->vector_datos_archivos[i].fmt_entrada = FMT_TXT;
			parametros->vector_datos_archivos[i].nombre_archivo = argv[i];
		}
	}
	return ST_OK;
}

void liberar_vector_archivos (archivo_t * archivos) {
	if (archivos)
		free(archivos);
}
		
