/** 
 * @file tda_lista_rec.c
 * @brief Contiene la implementación, 100% recursiva, de las primitivas de una
 * lista polimórfica simplemente enlazada.
 * @author PATRICIO MORENO
 * @date 2017-05-30
 * @license beerware
 * ---------------------------------------------------------------------------
 * "LA LICENCIA BEER-WARE" (Versión 42):
 * Yo escribí este archivo. Siempre y cuando usted mantenga este aviso, puede
 * hacer lo que quiera con él. Si nos encontramos algún día, y piensa que esto
 * vale la pena, me puede comprar una cerveza a cambio.        PATRICIO MORENO
 * ---------------------------------------------------------------------------
 */
#include <stdlib.h>
#include "../lib/types.h"
#include "tda_lista.h"

/* Recibe una lista. En caso de que esté vacía devuelve TRUE,
 * si no lo está devuelve FALSE */
bool_t LISTA_esta_vacia(lista_t lista) {
    return lista == NULL;
}
/* Recibe un puntero a una lista. Deuvelve por la interfaz una lista
 * vacía y por el nombre un estado */
retval_t LISTA_crear(lista_t * plista) {
    if(plista == NULL)
        return RV_ILLEGAL;

    *plista = NULL;

    return RV_SUCCESS;
}
/* Recibe un puntero doble a un nodo y un dato. Crea un nodo, guardando
 * el dato recibido y un puntero al siguiente nodo, inicializándolo en NULL,
 * indicando el final de la lista. Devuelve El nodo por la interfaz y un
 * estado por el nombre */
retval_t LISTA_crear_nodo(nodo_t ** pnodo, void * dato) {
    if(pnodo == NULL)
        return RV_ILLEGAL;

    if((*pnodo = (nodo_t *)calloc(1, sizeof(nodo_t))) == NULL)
        return RV_NOSPACE;

    (*pnodo)->siguiente = NULL;
    (*pnodo)->dato = dato;

    return RV_SUCCESS;
}
/* Recibe un puntero doble a un nodo y un puntero a una función que sabrá
 * destruir el dato almacenado. Guarda la dirección del dato almacenado 
 * en el nodo en un puntero a void y apunta a los punteros del nodo a
 * NULL, y luego libera el puntero al nodo. Finalmente, si el puntero a 
 * la función de destrucción no es nulo llama a la función y devuelve su
 * estado por el nombre; de ser nulo el puntero devuelve RV_SUCCESS por 
 * el nombre */
retval_t LISTA_destruir_nodo(nodo_t ** pnodo, retval_t (*destructor_dato)(void *)) {
    void * dato;

    if(pnodo == NULL)
        return RV_ILLEGAL;

    if(LISTA_esta_vacia(*pnodo))
        return RV_SUCCESS;

    dato = (*pnodo)->dato;

    (*pnodo)->siguiente = NULL;
    (*pnodo)->dato = NULL;
    free(*pnodo);
    *pnodo = NULL;

    return (destructor_dato != NULL) ? (*destructor_dato)(dato) : RV_SUCCESS;
}
/* Recibe un puntero a una lista y un puntero a una función que destruye
 * el dato al que apunta el nodo. Si el puntero al primer elemento apunta
 * a NULL se informa que la función finalizó con éxito por el nombre y se
 * devuelve la nueva lista por la interfaz.
 * Guarda la dirección a la que apunta el puntero al inicio de la lista
 * en un puntero a nodo_t y el puntero al principio de la lista se lo 
 * apunta al siguiente elemento de la lista. 
 * Al final se llama a la función LISTA_destruir_nodo y se destruye al 
 * nodo con esa función */
retval_t LISTA_destruir_primero(lista_t * plista, retval_t (*destructor_dato)(void *)) {
    nodo_t * primero;

    if(plista == NULL)
        return RV_ILLEGAL;

    if(*plista == NULL)/* es equivalente a LISTA_esta_vacia(*plista) */
        return RV_SUCCESS;

    primero = *plista;
    *plista = (*plista)->siguiente;

    return LISTA_destruir_nodo(&primero, destructor_dato);
}
/* Recibe un puntero a la lista y un puntero a una función que destruye el
 * dato al que apunta el nodo. Si la lista está vacía devuelve un estado
 * por el nombre y la lista vacía por la interfaz. De no estar vacía se
 * guarda el puntero al siguiente elemento de la lista, se destruye el nodo
 * actual con LISTA_destruir_nodo y se vuelve a llamar a la función con el
 * siguiente nodo de la lista */
retval_t LISTA_destruir(lista_t * plista, retval_t (*destructor_dato)(void *)) {
    nodo_t * siguiente;

    if(plista == NULL)
        return RV_ILLEGAL;

    if(LISTA_esta_vacia(*plista))
        return RV_SUCCESS;

    siguiente = (*plista)->siguiente;

    LISTA_destruir_nodo(plista, destructor_dato );
    return LISTA_destruir(&siguiente, destructor_dato);
}
/* Recibe un puntero a una lista y un puntero a un dato a insertar al
 * comienzo de la lista. Se crea un nodo en el cual se guarda el puntero
 * al dato y se lo apunta al primer elemento de la lista, también apuntando
 * el puntero al inicio de la lista al nuevo nodo.
 * Se devuelve la nueva lista por la interfaz y un estado por el nombre */
retval_t LISTA_insertar_al_ppio(lista_t * plista, void * dato) {
    nodo_t * nuevo;
    retval_t rv;

    if(plista == NULL)
        return RV_ILLEGAL;

    if((rv = LISTA_crear_nodo(&nuevo, dato)) != RV_SUCCESS)
        return rv;

    nuevo->siguiente = *plista;
    *plista = nuevo;

    return RV_SUCCESS;
}
/* Recibe un puntero a una lista y un puntero a un dato a insertar al final
 * de la lista. Si la lista está vacía se llama a la función que ingresa
 * al comienzo. Si no lo está, se vuelve a llamar a está función con el
 * puntero al siguiente elemento.
 * Devuelve un estado por el nombre y la lista actualizada por la interfaz */
retval_t LISTA_insertar_al_final(lista_t * plista, void * dato) {
    if(plista == NULL)
        return RV_ILLEGAL;

    if(LISTA_esta_vacia(*plista))
        return LISTA_insertar_al_ppio(plista, dato);

    return LISTA_insertar_al_final(&((*plista)->siguiente), dato);
}
/* Recibe un puntero a una lista previamente ordenada, un puntero a un
 * dato y una función que compara datos. Si la lista está vacía o si la
 * función de comparación es menor a 0 se crea un nodo, se guarda el
 * dato en ese nodo, y se inserta el nodo en el lugar previo al nodo en el
 * que se encontraba el puntero de la lista.
 * De otra forma, se vuelve a llamar a la función con el
 * siguiente elemento de la lista y se vuelve a preguntar. 
 * Devuelve la lista actualizada por la interfaz y un estado por el nombre */
retval_t LISTA_insertar_decreciente(lista_t * plista, void * dato, int (*cmp)(void *, void *)) {
    retval_t rv;
    nodo_t * nuevo;

    if(plista == NULL)
        return RV_ILLEGAL;

    if(LISTA_esta_vacia(*plista) || (*cmp)( (*plista)->dato, dato) < 0 ) {
        if((rv = LISTA_crear_nodo(&nuevo, dato)) != RV_SUCCESS)
            return rv;

        nuevo->siguiente = *plista;
        *plista = nuevo;

        return RV_SUCCESS;
    }

    return LISTA_insertar_decreciente(&((*plista)->siguiente), dato, cmp);
}
/* Recibe un puntero a un nodo, un dato a buscar y un puntero a una función
 * que indica si dos elementos son iguales. Llama a la función y si esta
 * es distinta de 0 devuelve un puntero al dato. De no ser iguales se vuelve
 * a llamar a LISTA_buscar con el puntero al siguiente nodo */
void * LISTA_buscar(lista_t pnodo, void * t, int (*es_igual)(void *, void *)) {
    if(pnodo == NULL)
        return NULL;

    if((*es_igual)(t, pnodo->dato))
        return pnodo->dato;

    return LISTA_buscar(pnodo->siguiente, t, es_igual);
}
/* Recibe un puntero a un nodo, el stream en donde imprimir y un puntero
 * a una función que imprime el tipo de dato guardado en la lista. Imprime
 * el dato llamando a la función pasada como argumento y luego vuelve a
 * llamar a LISTA_imprimir hasta que el puntero al nodo sea nulo. 
 * Devuelve un estado por el nombre */
retval_t LISTA_imprimir(lista_t pnodo, FILE * ofile, retval_t (*impresor)(void *, FILE *)) {
    if(pnodo == NULL)
        return RV_SUCCESS;

    (*impresor)(pnodo->dato, ofile);
    LISTA_imprimir(pnodo->siguiente, ofile, impresor);

    return RV_SUCCESS;
}
/* Recibe un puntero a un nodo, un puntero a una función la cual llevará
 * a cabo la operación deseada por el usuario y un puntero a un argumento
 * para dicha función. De no ser nulo el puntero al nodo, se llama a la
 * función pasada como argumento con el dato del nodo y el argumento pasado
 * como argumento. Se vuelve a llamar a LISTA_recorrer con el siguiente
 * puntero a nodo hasta que este sea nulo
 * Devuelve un estado por el nombre */
retval_t LISTA_recorrer(lista_t pnodo, retval_t (*funcion)(void *, void *), void * argumento) {
    if(pnodo == NULL)
        return RV_SUCCESS;

    (*funcion)(pnodo->dato, argumento);
    return LISTA_recorrer(pnodo->siguiente, funcion, argumento);
}
