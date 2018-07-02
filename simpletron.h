#ifndef SIMPLETRON__H
#define SIMPLETRON__H 1

#include <stdio.h>
#include <stdlib.h>
#include "tda_lista.h"

#define MAX_OPERANDO 511
#define MAX_OPCODE 63
#define MASK_OPCODE 0x7E00
#define SHIFT_OPCODE 9
#define MASK_OPERANDO 0x1FF
#define SHIFT_OPERANDO 0
#define MASK_SIGNO 0x8000
#define SHIFT_SIGNO 15
#define DIVISOR 10000
#define MAX_LARGO 150
#define CANTIDAD_DE_FUNCIONES 15 /*no cuenta halt*/
#define MAX_NUMERO 9999999
#define MIN_NUMERO -9999999
#define MIN_NUMERO_NEGATIVO_GUARDAR -32767
#define MASK_NUMERO_NEGATIVO 0x7FFF


typedef short int palabra_t;



typedef struct simpletron {
	struct vector * vector;
	size_t cantidad_de_memoria;
	size_t contador;
	palabra_t registro;
	long acc;
} simpletron_t;

typedef enum {
	ST_SMP_OK,
	ST_SMP_ERROR_PUNTERO_NULO,
	ST_SMP_ERROR_EJECUCION,
	ST_SMP_ERROR_LISTA,
	ST_SMP_ERROR_PALABRA
} status_simpletron;

typedef enum {
	LEER = 10,
	ESCRIBIR = 11,
	CARGAR = 20,
	GUARDAR = 21,
	PCARGAR = 22,
	PGUARDAR = 23,
	SUMAR = 30,
	RESTAR = 21,
	DIVIDIR = 32,
	MULTIPLICAR = 33,
	JMP = 40,
	JMPNEG = 41,
	JMPZERO = 42,
	JNZ = 43,
	DJNZ = 44,
	HALT = 45
} opcode_t;


typedef enum {
	SIGNO_POSITIVO,
	SIGNO_NEGATIVO
} signo_t;

typedef status_simpletron (*funcion_simpletron)(simpletron_t *);

typedef struct {
	funcion_simpletron funcion;
	opcode_t opcode;
} funcion_t;



/*esto no se si dejarlo, vamos viendo como se va desarrollando todo
typedef struct archivo {
	simpletron_t simpletron;
	char * nombre_archivo;
} archivo_t;*/



void palabra_guardar_opcode (palabra_t * palabra, int opcode);
void palabra_guardar_operando (palabra_t * palabra, int operando);
status_simpletron palabra_validar_generar (long numero, palabra_t * palabra);
signo_t palabra_leer_signo (palabra_t palabra);
size_t  palabra_leer_operando (palabra_t palabra);
long palabra_unir (palabra_t palabra);
opcode_t palabra_leer_opcode (palabra_t palabra);
void palabra_destruir (palabra_t * palabra);
palabra_t * palabra_copiar (palabra_t * dato);
status_simpletron simpletron_cargar (simpletron_t * simpletron);
status_simpletron simpletron_leer (simpletron_t * simpletron);
status_simpletron simpletron_escribir (simpletron_t * simpletron);
status_simpletron simpletron_guardar (simpletron_t * simpletron);
status_simpletron simpletron_pcargar (simpletron_t * simpletron);
status_simpletron simpletron_pguardar (simpletron_t * simpletron);
status_simpletron simpletron_sumar (simpletron_t * simpletron);
status_simpletron simpletron_restar (simpletron_t * simpletron);
status_simpletron simpletron_multiplicar (simpletron_t * simpletron);
status_simpletron simpletron_dividir (simpletron_t * simpletron);
status_simpletron simpletron_jmp (simpletron_t * simpletron);
status_simpletron simpletron_jmpneg (simpletron_t * simpletron);
status_simpletron simpletron_jmpzero (simpletron_t * simpletron);
status_simpletron simpletron_jnz (simpletron_t * simpletron);
status_simpletron simpletron_djnz (simpletron_t * simpletron);
void cargar_vector_funciones (funcion_t * vector);
funcion_simpletron buscar_vector_funciones (funcion_t * vector, opcode_t opcode);
void simpletron_borrar (simpletron_t * simpletron);
simpletron_t * simpletron_crear (size_t memoria_vector);
status_simpletron simpletron_ejecutar_lista (lista_t lista);
status_simpletron simpletron_ejecutar_individual (simpletron_t * simpletron, funcion_t * funciones);

#endif
