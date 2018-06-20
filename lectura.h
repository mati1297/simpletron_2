#ifndef LECTURA__H
#define LECTURA__H

#include <stdio.h>
#include "simpletron.h"
#include "vector.h"
#include "herramientas.h"
#include "error.h"

#define MAX_LARGO 150
#define DELIMITADOR_COMENTARIO ';'


status_t leer_guardar (archivos * archivo, vector_t * vector);

#endif
