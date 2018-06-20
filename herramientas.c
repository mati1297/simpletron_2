#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "herramientas.h"

char * recortar_espacios (char * s) {
	char * inicio, * fin;
	for (inicio = s; isspace (*inicio) && *inicio; inicio++)
		;
	if (!(*inicio)) {
		*s = '\0';
		return s;
	}
	for (fin = s + strlen (s) - 1; isspace (*fin) && fin != s; fin--)
		;
	*++fin = '\0';
	return memmove (s, inicio, fin - inicio + 1);
}

void cortar_delimitador (char * s, char delimitador) {
	char * busqueda;
	busqueda = strchr(s, delimitador);
	if(busqueda)
		*busqueda = '\0';
}
