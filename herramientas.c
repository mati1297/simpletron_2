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
	busqueda = strchr (s, delimitador);
	if (busqueda)
		*busqueda = '\0';
}

status_t imprimir_ayuda (FILE * f_out) {
	FILE * fhelp;
	char * buffer;
	if (!(fhelp = fopen (ARCHIVO_AYUDA, "rt"))) 
		return ST_ERROR_LECTURA_ARCHIVO;
	
	if (!(buffer = (char *) malloc (sizeof (char) * MAX_CADENA))) {
		fclose (fhelp);
		return ST_ERROR_MEMORIA_INVALIDA;
	}
	while (fgets (buffer, MAX_CADENA + 2, fhelp))
		fprintf (f_out, "%s", buffer);
	free (buffer);
	if (ferror (fhelp)) {
		fclose (fhelp);
		return ST_ERROR_LECTURA_ARCHIVO;
	}
	fclose (fhelp);
	return ST_OK;
}
