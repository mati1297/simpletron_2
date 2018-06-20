#include <stdio.h>
#include "simpletron.h"

/*esta funcion supone que los valores son correctos*/
void palabra_guardar_opcode (palabra_t * palabra, int opcode) {
	(*palabra) &= ~MASK_OPCODE;
	(*palabra) |= opcode << SHIFT_OPCODE;
}

void palabra_guardar_operando (palabra_t * palabra, int operando) {
	(*palabra) &= ~MASK_OPERANDO;
	(*palabra) |= operando << SHIFT_OPERANDO;
}
