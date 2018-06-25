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

bool_t LISTA_esta_vacia(lista_t lista) {
    return lista == NULL;
}

retval_t LISTA_crear(lista_t * plista) {
    if(plista == NULL)
        return RV_ILLEGAL;

    *plista = NULL;

    return RV_SUCCESS;
}

retval_t LISTA_crear_nodo(nodo_t ** pnodo, void * dato) {
    if(pnodo == NULL)
        return RV_ILLEGAL;

    if((*pnodo = (nodo_t *)calloc(1, sizeof(nodo_t))) == NULL)
        return RV_NOSPACE;

    (*pnodo)->siguiente = NULL;
    (*pnodo)->dato = dato;

    return RV_SUCCESS;
}

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

retval_t LISTA_insertar_al_final(lista_t * plista, void * dato) {
    if(plista == NULL)
        return RV_ILLEGAL;

    if(LISTA_esta_vacia(*plista))
        return LISTA_insertar_al_ppio(plista, dato);

    return LISTA_insertar_al_final(&((*plista)->siguiente), dato);
}

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

void * LISTA_buscar(lista_t pnodo, void * t, int (*es_igual)(void *, void *)) {
    if(pnodo == NULL)
        return NULL;

    if((*es_igual)(t, pnodo->dato))
        return pnodo->dato;

    return LISTA_buscar(pnodo->siguiente, t, es_igual);
}

retval_t LISTA_imprimir(lista_t pnodo, FILE * ofile, retval_t (*impresor)(void *, FILE *)) {
    if(pnodo == NULL)
        return RV_SUCCESS;

    (*impresor)(pnodo->dato, ofile);
    LISTA_imprimir(pnodo->siguiente, ofile, impresor);

    return RV_SUCCESS;
}

retval_t LISTA_recorrer(lista_t pnodo, retval_t (*funcion)(void *, void *), void * argumento) {
    if(pnodo == NULL)
        return RV_SUCCESS;

    (*funcion)(pnodo->dato, argumento);
    return LISTA_recorrer(pnodo->siguiente, funcion, argumento);
}
