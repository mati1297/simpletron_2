#ifndef SIMPLETRON__H
#define SIMPLETRON__H 1

#define MAX_OPERANDO 99
#define MASK_OPCODE 0xFE00
#define SHIFT_OPCODE 9
#define MASK_OPERANDO 0x1FF
#define SHIFT_OPERANDO 0

typedef short int palabra_t;

typedef struct simpletron {
	palabra_t ** vector;
	size_t cantidad_de_memoria;
	palabra_t registro;
	palabra_t acc;
} simpletron_t;

/*esto no se si dejarlo, vamos viendo como se va desarrollando todo
typedef struct {
	simpletron_t simpletron;
	char * nombre_archivo;
} archivo_t;*/

void palabra_guardar_opcode (palabra_t * palabra, int opcode);
void palabra_guardar_operando (palabra_t * palabra, int operando);

#endif
