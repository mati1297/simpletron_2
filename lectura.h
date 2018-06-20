#ifndef LECTURA__H
#define LECTURA__H

#include "herramientas.h"

#define MAX_LARGO 150
#define DELIMITADOR_COMENTARIO ';'
#define DIVISOR 10000

status_t leer_guardar (archivos * archivo, vector_t * vector);

#endif
