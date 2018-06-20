#ifndef VECTOR__H
#define VECTOR__H 1

#include "simpletron.h"
#include "error.h"


typedef struct vector {
	size_t pedido;
	palabra_t ** datos;
} vector_t;

/*AXIOMA: un vector vacio es datos = NULL*/


typedef enum {
	ST_VEC_OK,
	ST_VEC_ERROR_PUNTERO_NULO,
	ST_VEC_ERROR_POSICION,
	ST_VEC_ERROR_NO_MEMORIA
} status_vector;

vector_t * vector_crear (size_t size);
void vector_destruir (vector_t ** v, void (*destruir) (palabra_t *));
status_vector vector_guardar (vector_t * v, size_t i, palabra_t * dato, palabra_t * (*copiar)(palabra_t*), void(*destruir)(palabra_t *));
const palabra_t * vector_leer (const vector_t * v, size_t i);
status_vector vector_iterar (vector_t * v, size_t inicio, size_t fin, void(*funcion)(void *, void *), void * arg);
size_t vector_memoria (vector_t * v);

#endif
