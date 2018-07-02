#ifndef HERRAMIENTAS__H
#define HERRAMIENTAS__H 1

#include "error.h"
#include "idioma.h"

#define MAX_CADENA 150

char * recortar_espacios (char * s);
void cortar_delimitador (char * s, char delimitador);
status_t imprimir_ayuda (FILE * f_out);

#endif
