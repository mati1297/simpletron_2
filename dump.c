#include <stdio.h>
#include <ctype.h>
#include "error.h"
#include "tda_lista.h"
#include "simpletron.h"
#include "vector.h"
#include "dump.h"

/* Recibe un puntero al comienzo de una lista y el formato en el que se desea imprimir el dump
 * En el caso que se quiera formato texto, se imprime con el formato indicado en la consigna del TP.
 * Si se quiere binario, se imprimen los datos necesarios uno atras de otro sin espacios ni comentarios.
 * Abre los archivos con el formato indicado utilizando un nombre standard.
 * Por el nombre devuelve un estado que indica si fallo o no la funcion*/
status_t imprimir_dump(lista_t lista, fmt_t formato){
	FILE *f_out;
		if(formato == FMT_TXT) {/*si el formato de salida es txt*/
			if(!(f_out = fopen(NOMBRE_ARCHIVO_STANDARD_TXT, "wt")))
				return ST_ERROR_LECTURA_ARCHIVO;
			if(LISTA_recorrer(lista, &imprimir, f_out) != RV_SUCCESS) {
				fclose(f_out);
				return ST_ERROR_LISTA;
			}
			
		}
			/*while(!(lista->siguiente)){
				if((st = imprimir_registros (lista->dato, f_out)) != ST_OK) {
					fclose (f_out);
					return st;
				}
				if((st = imprimir_memoria (lista->dato->vector, lista->dato->cantidad_de_memoria, f_out)) != ST_OK) {
					fclose (f_out);
					return st;
				}
				
				lista->siguiente = lista->siguiente->siguiente;
			}*/
		else { /*si el formato de salida es binario*/
			if(!(f_out = fopen(NOMBRE_ARCHIVO_STANDARD_BIN, "wb")))
				return ST_ERROR_LECTURA_ARCHIVO;
			if(LISTA_recorrer(lista, &imprimir_bin, f_out) != RV_SUCCESS) {
				fclose(f_out);
				return ST_ERROR_LISTA;
			}
			
			/*while(!(lista -> siguiente)){
				if((st = imprimir_bin(lista->dato, f_out, lista->dato->cantidad_de_memoria)) != ST_OK) {
					fclose (f_out);
					return st;
				}
				lista->siguiente = lista->siguiente->siguiente;
			}*/
		}
		fclose(f_out);
	return ST_OK;
}

status_t imprimir (const simpletron_t * simpletron, FILE * f_output) {
	status_t st;
	if((st = imprimir_registros(simpletron, f_output)) != ST_OK)
		return st;
	if((st = imprimir_memoria(simpletron, f_output)) != ST_OK)
		return st;
	return ST_OK;
}

/*Recibe un puntero a la estructura simpletron_t y otro al archivo de salida.
 * Imprime en formato texto la parte de los registros de la ejecucion 
 * del simpletron. Devuelve estado por el nombre.*/
status_t imprimir_registros (const simpletron_t * simpletron, FILE * f_output) {
	if (simpletron == NULL || f_output == NULL)
		return ST_ERROR_PUNTERO_NULO;
	fprintf(f_output, "%s:\n\n", MSJ_REGISTROS);
	fprintf(f_output, "%16s", MSJ_ACUMULADOR);
	fprintf(f_output, "%9X\n", simpletron->acc);
	fprintf(f_output, "%16s", MSJ_CONTADOR);
	fprintf(f_output, "%9lu\n", simpletron->contador);
	fprintf(f_output, "%16s", MSJ_INSTRUCCION);
	fprintf(f_output, "%+9ld\n", palabra_unir(simpletron->registro));
	fprintf(f_output, "%16s", MSJ_OPCODE);
	fprintf(f_output, "%16d\n", palabra_leer_opcode(simpletron->registro));
	fprintf(f_output, "%16s", MSJ_OPERANDO);
	fprintf(f_output, "%16lu\n", palabra_leer_operando(simpletron->registro));
	fputc('\n', f_output);
	return ST_OK;
}

/*Recibe un puntero a la estructura simpletron, la cantidad de memoria del programa y 
 * un puntero al archivo de salida. Imprime en formato texto los datos cargados 
 * en memoria al final de la ejecucion del simpletron y los mismos representados en codigo ASCII en forma de tabla. 
 * La memoria es de tipo palabra_t por lo que a cada una le corresponden 2 chars.
 * Entoces por cada palabra, se imprimen dos caracteres ascii.
 * Si se tiene un caracter no imprimible, se imprime un punto.
 * Devuelve el estado por el nombre*/
/*status_t imprimir_memoria (const simpletron_t * simpletron, FILE * f_output) {
	size_t i;
	int i_binario;
	if (f_output == NULL)
		return ST_ERROR_PUNTERO_NULO;
	fprintf(f_output,"%s", MSJ_MEMORIA);
	if(vector_esta_vacio(simpletron->vector))
		return ST_OK;
	for(i = 0; i < simpletron->cantidad_de_memoria; i++){
		/*if(!(i % CANT_COLS)){
			fputc('\n',f_output);
			i_binario = (int) i;
			printf("%lu", i);
			fprintf(f_output, "%04x0:  ", i_binario);
			
			imprimir_ascii(simpletron->vector, i, f_output);
		}
		
		fprintf(f_output, "%hu  ", *(vector_leer(simpletron->vector, i)));
	}
	return ST_OK;
}*/

status_t imprimir_memoria (const simpletron_t * simpletron, FILE * f_output) {
	size_t i;
	const palabra_t * aux;
	palabra_t hola;
	if(f_output == NULL)
		return ST_ERROR_PUNTERO_NULO;
	if(vector_esta_vacio(simpletron->vector))
		return ST_OK;
	fprintf(f_output,"%S\n", MSJ_MEMORIA); 
	fputc('\n', f_output);
	for(i = 0; i < simpletron->cantidad_de_memoria; i++) {
		if(!(i % CANT_COLS)) {
			imprimir_ascii(simpletron->vector, i, f_output);
			fputc('\n', f_output);
			fprintf(f_output, "%03X:  ", i);
		}
		aux = vector_leer(simpletron->vector, i);
		hola = *aux;
		fprintf(f_output, "%04X  ", hola);
	}
	fputc('\n', f_output);
	fputc('\n', f_output);
	return ST_OK;
}

/*Recibe un puntero a la estructura vector_t y un entero para iterar.
 * Imprime la representacion ascii de la memoria en el mismo renglon.
 * En el caso de que se tenga un caracter no imprimible, se lo reemplaza por un punto*/
void imprimir_ascii(const vector_t * vector_memoria, int i, FILE * f_output) {
	palabra_t aux;
	size_t j;
	char c;
	for(j = i - CANT_COLS; j < i; j++) {
		aux = *(vector_leer(vector_memoria, j));
		c = (aux & MASK_CHAR) >> SHIFT_CHAR;
		fputc((isprint(c))?c:CARAC_NO_IMPRIMIBLE, f_output);
		c = (aux & ~MASK_CHAR);
		fputc((isprint(c))?c:CARAC_NO_IMPRIMIBLE, f_output);
	} 
}

/*Recibe un puntero a una estructura simpletron de donde obtendra los datos a imprimir,
 * un archivo en el cual imprimira y la cant_memoria a imprimir.
 * Imprime en formato BigEndian unicamente la memoria de simpletron.
 * Devuelve por el nombre un estado que indica si hubo algun error y cual fue*/
status_t imprimir_bin(const simpletron_t * simpletron, FILE * f_output){
	int i;
	if(!f_output || !simpletron)
		return ST_ERROR_PUNTERO_NULO;
	for (i = 0; i < simpletron->cantidad_de_memoria; i++){
		if((fwrite(&(simpletron->vector[i]),sizeof(palabra_t), 1, f_output)) != 1)
			return ST_ERROR_LECTURA_ARCHIVO;
	}
	return ST_OK;
}
