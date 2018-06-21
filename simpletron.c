#include <stdio.h>
#include <stdlib.h>
#include "simpletron.h"
#include "vector.h"

/*esta funcion supone que los valores son correctos*/
void palabra_guardar_opcode (palabra_t * palabra, int opcode) {
	(*palabra) &= ~MASK_OPCODE;
	(*palabra) |= opcode << SHIFT_OPCODE;
}

void palabra_guardar_operando (palabra_t * palabra, int operando) {
	(*palabra) &= ~MASK_OPERANDO;
	(*palabra) |= operando << SHIFT_OPERANDO;
}

palabra_t palabra_leer_opcode (palabra_t palabra) {
	return (palabra & MASK_OPCODE) >> SHIFT_OPCODE;
}

palabra_t palabra_leer_operando (palabra_t palabra) {
	return (palabra & MASK_OPERANDO) >> SHIFT_OPERANDO;
}

int palabra_unir (palabra_t palabra) {
	return palabra_leer_opcode (palabra) * DIVISOR + palabra_leer_operando (palabra);
}

palabra_t palabra_dividir (int numero) {
	palabra_t palabra;
	palabra_guardar_opcode (&palabra, numero / DIVISOR);
	palabra_guardar_opcode (&palabra, numero % DIVISOR);
	return palabra;
}

palabra_t * palabra_copiar (palabra_t * dato) {
	palabra_t * palabra;
	if (!dato)
		return NULL;
	if (!(palabra = (palabra_t *) calloc (1, sizeof (palabra_t))))
		return NULL;
	*palabra = *dato;
	return palabra;
}

void palabra_destruir (palabra_t * palabra) {
	if (palabra)
		free (palabra);
}


