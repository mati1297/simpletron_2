#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "simpletron.h"

/* CUANDO SE HAGAN LAS FUNCIONES LAS CUALES ACA SE USA SU PUNTERO, HAY QUE MODIFICAR QUE PARAMETROS TIENEN (TIPOS,ETC)*/

vector_t * vector_crear (size_t size){
	vector_t * v;
	if(!(v = (vector_t *) calloc (1, sizeof(vector_t))))
		return NULL;
	if(!(v->datos = (palabra_t **) calloc (size, sizeof(palabra_t *)))) {
		free(v);
		return NULL;
	}
	v->pedido = size;
	return v;
}

void vector_destruir (vector_t ** v, void (*destruir) (palabra_t *)) {
	size_t i;
	if(v && *v) {
		if(*v) {
			if((*v)->datos) {
				for(i = 0; i < (*v)->pedido; i++) {
					(*destruir)((*v)->datos[i]);
				}
			}
			free((*v)->datos);
		}
		free(*v);
	}
}

status_vector vector_guardar (vector_t * v, size_t i, palabra_t * dato, palabra_t * (*copiar)(palabra_t*), void(*destruir)(palabra_t *)) {
	palabra_t * aux;
	if (!v)
		return ST_VEC_ERROR_PUNTERO_NULO;
	if (i >= v->pedido)
		return ST_VEC_ERROR_POSICION;
	if(!(aux = (*copiar)(dato)))
		return ST_VEC_ERROR_NO_MEMORIA;
	if (v->datos[i])
		(*destruir)(v->datos[i]);
	v->datos[i] = aux;
	return ST_VEC_OK;
} 

const palabra_t * vector_leer (const vector_t * v, size_t i) {
	if (!v)
		return NULL;
	if (i >= v->pedido)
		return NULL;
	return v->datos[i];
}


status_vector vector_iterar (vector_t * v, size_t inicio, size_t fin, void(*funcion)(void *, void *), void * arg) {
	size_t i;
	if (!v || funcion)
		return ST_VEC_ERROR_PUNTERO_NULO;
	if (inicio > fin || fin >= v->pedido)
		return ST_VEC_ERROR_POSICION;
	for (i = inicio; i <= fin; i++)
		if(v->datos[i])
			(*funcion)(arg, v->datos[i]);
	return ST_VEC_OK;
}

size_t vector_memoria (vector_t * v) {
	return v->pedido;
}


