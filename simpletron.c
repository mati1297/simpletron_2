#include <stdio.h>
#include <stdlib.h>
#include "simpletron.h"
#include "vector.h"
#include "tda_lista.h"


/*Funcion que se encarga de cargar en el vector de punteros a funcion todas
 * las funciones y sus opcodes para que sean facilmente llamadas. No se incluye
 * el opcode HALT ya que no hay una funcion explicita definida para este opcode*/
void cargar_vector_funciones (funcion_t * vector) {
	vector[0].opcode = LEER;
	vector[0].funcion = &simpletron_leer;
	vector[1].opcode = ESCRIBIR;
	vector[1].funcion = &simpletron_escribir;
	vector[2].opcode = CARGAR;
	vector[2].funcion = &simpletron_cargar;
	vector[3].opcode = GUARDAR;
	vector[3].funcion = &simpletron_guardar;
	vector[4].opcode = PCARGAR;
	vector[4].funcion = &simpletron_pcargar;
	vector[5].opcode = PGUARDAR;
	vector[5].funcion = &simpletron_pguardar;
	vector[6].opcode = SUMAR;
	vector[6].funcion = &simpletron_sumar;
	vector[7].opcode = RESTAR;
	vector[7].funcion = &simpletron_restar;
	vector[8].opcode = DIVIDIR;
	vector[8].funcion = &simpletron_dividir;
	vector[9].opcode = MULTIPLICAR;
	vector[9].funcion = &simpletron_multiplicar;
	vector[10].opcode = JMP;
	vector[10].funcion = &simpletron_jmp;
	vector[11].opcode = JMPNEG;
	vector[11].funcion = &simpletron_jmpneg;
	vector[12].opcode = JMPZERO;
	vector[12].funcion = &simpletron_jmpzero;
	vector[13].opcode = JNZ;
	vector[13].funcion = &simpletron_jnz;
	vector[14].opcode = DJNZ;
	vector[14].funcion = &simpletron_djnz;
}


/*Ejecuta todos los simpletron's de cada programa, para ello primero itera entre los
 * distintos nodos de la lista, y a su vez entre las ordenes de cada programa.
 * Esto ultimo lo hace buscando la funcion que le corresponde al opcode con la funcion
 * buscar_vector_funciones que devuelve un puntero a funcion*/
status_simpletron ejecutar_simpletron (lista_t lista) {
	funcion_t funciones[CANTIDAD_DE_FUNCIONES];
	funcion_simpletron funcion;
	simpletron_t * simpletron;
	const palabra_t * aux;
	opcode_t opcode;
	if(!lista)
		return ST_SMP_ERROR_PUNTERO_NULO;
	cargar_vector_funciones(funciones);
	/*itera mientras la lista no sea nula*/
	while(lista) {
		simpletron = (simpletron_t *) lista->dato;
		for(simpletron->contador = 0;;) {
			if(!(aux = vector_leer(simpletron->vector, simpletron->contador)))
				return ST_SMP_ERROR_EJECUCION;
			simpletron->registro = *aux;
			if((opcode = palabra_leer_opcode(simpletron->registro) == HALT))
				break;
			if(!(funcion = buscar_vector_funciones(funciones, opcode)))
				return ST_SMP_ERROR_EJECUCION;
			(*funcion) (simpletron);
		}
		lista = lista->siguiente;
	}
	return ST_SMP_OK;
}

/*Se encarga de buscar el puntero a funcion correspondiente segun el opcode que se pasa
 * en la estructura de funciones. Si no lo encuentra, la funcion retorna NULL*/
funcion_simpletron buscar_vector_funciones (funcion_t * vector, opcode_t opcode) {
	size_t i;
	if(!vector)
		return NULL;
	for (i = 0; i < CANTIDAD_DE_FUNCIONES; i++) {
		if(opcode == vector[i].opcode)
			return vector[i].funcion;
	}
	return NULL;
}

/*Carga el numero indicado por el operando del registro en el acumulador, 
 * devuelve el estado por nombre*/
status_simpletron simpletron_cargar (simpletron_t * simpletron) {
	const palabra_t * dato;
	if(!(dato = vector_leer(simpletron->vector, palabra_leer_operando(simpletron->registro))))
		return ST_SMP_ERROR_EJECUCION;
	simpletron->acc = palabra_unir(*dato);
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Lee de stdin y guarda en la posicion indicada por el operando
 * del registro, devuelve el estado por el nombre*/
status_simpletron simpletron_leer (simpletron_t * simpletron) {
	char buffer [MAX_LARGO], * endp;
	palabra_t dato;
	if(!fgets(buffer, MAX_LARGO, stdin))
		return ST_SMP_ERROR_EJECUCION;
	dato = palabra_dividir((int) strtol(buffer, &endp, 10));
	if(*endp)
		return ST_SMP_ERROR_EJECUCION;
	if(vector_guardar(simpletron->vector, palabra_leer_operando(simpletron->registro), &dato, &palabra_copiar, &palabra_destruir) != ST_VEC_OK)
		return ST_SMP_ERROR_EJECUCION;
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Imprime la posicion de memoria indicada por el operando del registro. Devuelve
 * el estado por el nombre*/
status_simpletron simpletron_escribir (simpletron_t * simpletron) {
	const palabra_t * dato;
	if(!(dato = vector_leer(simpletron->vector, palabra_leer_operando(simpletron->registro))));
		return ST_SMP_ERROR_EJECUCION;
	printf("%s %d:+07%d\n", MSJ_POSICION, palabra_leer_operando(simpletron->registro), palabra_unir(*dato));
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Guarda el acumulador en el la posicion de memoria indicada por el operando del registro.
 * Devuelve el estado por el nombre*/
status_simpletron simpletron_guardar (simpletron_t * simpletron) {
	palabra_t dato;
	dato = palabra_dividir(simpletron->acc);
	if(vector_guardar(simpletron->vector, palabra_leer_operando(simpletron->registro), &dato, &palabra_copiar, &palabra_destruir) != ST_VEC_OK)
		return ST_SMP_ERROR_EJECUCION;
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Carga en el acumulador la posicion de memoria que indica el numero que se encuentra 
 * en la posicion de memoria indicada por el operando del registro. Devuelve el 
 * estado por el nombre*/
status_simpletron simpletron_pcargar (simpletron_t * simpletron) {
	const palabra_t * dato;
	if(!(dato = vector_leer(simpletron->vector, palabra_leer_operando(simpletron->registro))))
		return ST_SMP_ERROR_EJECUCION;
	if(!(dato = vector_leer(simpletron->vector, palabra_leer_operando(*dato))))
		return ST_SMP_ERROR_EJECUCION;
	simpletron->acc = palabra_unir(*dato);
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Carga el acumulador en la posicion de memoria que indica el numero que se encuentra 
 * en la posicion de memoria indicada por el operando del registro. Devuelve el 
 * estado por el nombre*/
status_simpletron simpletron_pguardar (simpletron_t * simpletron) {
	const palabra_t * direccion;
	palabra_t dato;
	if(!(direccion = vector_leer(simpletron->vector, palabra_leer_operando(simpletron->registro))))
		return ST_SMP_ERROR_EJECUCION;
	dato = palabra_dividir(simpletron->acc);
	if(vector_guardar(simpletron->vector, palabra_leer_operando(*direccion), &dato, &palabra_copiar, &palabra_destruir) != ST_VEC_OK)
		return ST_SMP_ERROR_EJECUCION;
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Suma al acumulador la posicion de memoria indicada por el operando del registro. Devuelve el 
 * estado por el nombre*/
status_simpletron simpletron_sumar (simpletron_t * simpletron) {
	const palabra_t * dato;
	if(!(dato = vector_leer(simpletron->vector, palabra_leer_operando(simpletron->registro))))
		return ST_SMP_ERROR_EJECUCION;
	simpletron->acc += palabra_unir(*dato);
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Resta al acumulador la posicion de memoria indicada por el operando del registro. Devuelve el 
 * estado por el nombre*/
status_simpletron simpletron_restar (simpletron_t * simpletron) {
	const palabra_t * dato;
	if(!(dato = vector_leer(simpletron->vector, palabra_leer_operando(simpletron->registro))))
		return ST_SMP_ERROR_EJECUCION;
	simpletron->acc -= palabra_unir(*dato);
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Divide al acumulador por la posicion de memoria indicada por el operando del registro. Devuelve el 
 * estado por el nombre*/
status_simpletron simpletron_dividir (simpletron_t * simpletron) {
	const palabra_t * dato;
	if(!(dato = vector_leer(simpletron->vector, palabra_leer_operando(simpletron->registro))))
		return ST_SMP_ERROR_EJECUCION;
	simpletron->acc /= palabra_unir(*dato);
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Multiplica al acumulador por la posicion de memoria indicada por el operando del registro. Devuelve el 
 * estado por el nombre*/
status_simpletron simpletron_multiplicar (simpletron_t * simpletron) {
	const palabra_t * dato;
	if(!(dato = vector_leer(simpletron->vector, palabra_leer_operando(simpletron->registro))))
		return ST_SMP_ERROR_EJECUCION;
	simpletron->acc *= palabra_unir(*dato);
	simpletron->contador++;
	return ST_SMP_OK;
}

/*Salta a la posicion que indica el operando del registro. Devuelve el estado por el nombre*/
status_simpletron simpletron_jmp (simpletron_t * simpletron) {
	size_t posicion;
	if((posicion = palabra_leer_operando(simpletron->registro)) >= simpletron->cantidad_de_memoria)
		return ST_SMP_ERROR_EJECUCION;
	simpletron->contador = posicion;
	return ST_SMP_OK;
}

/*Salta a la posicion que indica el operando del registro si el acumulador es negativo. 
 * Para ello llama a la funcion simpletron_jmp. Devuelve el estado por el nombre*/
status_simpletron simpletron_jmpneg (simpletron_t * simpletron) {
	status_simpletron st;
	if(!simpletron)
		return ST_SMP_ERROR_PUNTERO_NULO;
	if(simpletron->acc < 0)
		if((st = simpletron_jmp(simpletron)) != ST_SMP_OK)
			return st;
	return ST_SMP_OK;
}

/*Salta a la posicion que indica el operando del registro si el acumulador es cero. 
 * Para ello llama a la funcion simpletron_jmp. Devuelve el estado por el nombre*/
status_simpletron simpletron_jmpzero (simpletron_t * simpletron) {
	status_simpletron st;
	if(simpletron->acc == 0)
		if((st = simpletron_jmp(simpletron)) != ST_SMP_OK)
			return st;
	return ST_SMP_OK;
}

/*Salta a la posicion que indica el operando del registro si el acumulador no es cero. 
 * Para ello llama a la funcion simpletron_jmp. Devuelve el estado por el nombre*/
status_simpletron simpletron_jnz (simpletron_t * simpletron) {
	status_simpletron st;
	if(simpletron->acc != 0)
		if((st = simpletron_jmp(simpletron)) != ST_SMP_OK)
			return st;
	return ST_SMP_OK;
}

/*Decrementa el acumulador y salta a la posicion que indica el operando si el acumulador
 * es cero. Para ello llama a la funcion simpletron_jmp. Devuelve el estado por el nombre*/
status_simpletron simpletron_djnz (simpletron_t * simpletron) {
	status_simpletron st;
	if((--(simpletron->acc)) != 0)
		if((st = simpletron_jmp(simpletron)) != ST_SMP_OK)
			return st;
	return ST_SMP_OK;
}

/*NO LAS COMENTO AUN PORQUE NO SE SI VAN A QUEDAR ASI POR EL TEMA DE COMO SE GUARDAN LOS NUMEROS, ETC*/
/*esta funcion supone que los valores son correctos*/
void palabra_guardar_opcode (palabra_t * palabra, int opcode) {
	(*palabra) &= ~MASK_OPCODE;
	opcode &= MASK_OPCODE;
	(*palabra) |= opcode << SHIFT_OPCODE;
}

void palabra_guardar_operando (palabra_t * palabra, int operando) {
	(*palabra) &= ~MASK_OPERANDO;
	operando &= MASK_OPERANDO;
	(*palabra) |= operando << SHIFT_OPERANDO;
}

opcode_t palabra_leer_opcode (palabra_t palabra) {
	return (palabra & MASK_OPCODE) >> SHIFT_OPCODE;
}

size_t palabra_leer_operando (palabra_t palabra) {
	return (palabra & MASK_OPERANDO) >> SHIFT_OPERANDO;
}

int palabra_unir (palabra_t palabra) {
	return palabra_leer_opcode (palabra) * DIVISOR + palabra_leer_operando (palabra);
}

palabra_t palabra_dividir (int numero) {
	palabra_t palabra;
	palabra_guardar_opcode (&palabra, numero / DIVISOR);
	palabra_guardar_operando (&palabra, numero % DIVISOR);
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

simpletron_t * simpletron_crear (size_t memoria_vector) {
	simpletron_t * simpletron;
	simpletron = (simpletron_t *) calloc (1,  sizeof(simpletron_t));
	if(!(simpletron->vector = vector_crear(memoria_vector)))
		free(simpletron);
	return simpletron;
}
/*ACA OJO QUE JODE LO DE RETVAL XD*/
void simpletron_borrar (simpletron_t * simpletron) {
	if(simpletron) {
		if(simpletron->vector)
			vector_destruir(&(simpletron->vector), &palabra_destruir);
	}
}





