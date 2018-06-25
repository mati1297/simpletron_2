/**
 * @file tda_lista_ite.c
 * @brief Contiene la implementación, 100% iterativa, de las primitivas de una
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
    nodo_t * nodo_a_borrar;
    retval_t rv;

    if(plista == NULL)
        return RV_ILLEGAL;

    while(*plista != NULL) {/* mientras la lista NO esté vacía */
        nodo_a_borrar = *plista;
        *plista = (*plista)->siguiente;
        if((rv = LISTA_destruir_nodo(&nodo_a_borrar, destructor_dato)) != RV_SUCCESS) {
            return rv;
        }
    }

    return RV_SUCCESS;
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
    retval_t rv;
    nodo_t * nuevo, *temp;

    if(plista == NULL)
        return RV_ILLEGAL;

    if((rv = LISTA_crear_nodo(&nuevo, dato)) != RV_SUCCESS)
        return rv;

    if(*plista == NULL) {/* es equivalente a llamar a LISTA_esta_vacia(*plista)*/
        *plista = nuevo;
    } else {
        for(temp = *plista; temp->siguiente != NULL; temp = temp->siguiente)
            ;
        temp->siguiente = nuevo;
    }

    return RV_SUCCESS;
}

retval_t LISTA_insertar_decreciente(lista_t * plista, void * dato, int (*cmp)(void *, void *)) {
    retval_t rv;
    nodo_t * nuevo, * actual, *anterior;

    if(plista == NULL)
        return RV_ILLEGAL;

    if((rv = LISTA_crear_nodo(&nuevo, dato)) != RV_SUCCESS)
        return rv;

    /* caso: lista vacía o inserción al ppio */
    if(*plista == NULL || (*cmp)((*plista)->dato, dato) < 0) {
        nuevo->siguiente = *plista;
        *plista = nuevo;
        return RV_SUCCESS;
    }

    /* otros casos: inserción al final o en puntos medios */
    for(actual = (*plista)->siguiente, anterior = *plista;
        actual != NULL;
        anterior = actual, actual = actual->siguiente) {
        if((*cmp)(actual->dato, dato) < 0)
            break;
    }

    anterior->siguiente = nuevo;
    nuevo->siguiente = actual;

    return RV_SUCCESS;
}

void * LISTA_buscar(lista_t plista, void * t, int (*es_igual)(void *, void *)) {
    if(plista == NULL || es_igual == NULL)
        return NULL;

    while(plista != NULL) {
        if((*es_igual)(t, plista->dato))
            return plista->dato;
        plista = plista->siguiente;
    }

    return NULL;
}

retval_t LISTA_imprimir(lista_t plista, FILE * ofile, retval_t (*impresor)(void *, FILE *)) {
    if(impresor != NULL) {
        while(plista != NULL) {
            (*impresor)(plista->dato, ofile);
            plista = plista->siguiente;
        }
    }

    return RV_SUCCESS;
}

/* con esta implementación de XX_recorrer, se puede imprimir también */
retval_t LISTA_recorrer(lista_t plista, retval_t (*funcion)(void *, void *), void * argumento) {
    if(funcion != NULL) {
        while(plista != NULL) {
            (*funcion)(plista->dato, argumento);
            plista = plista->siguiente;
        }
    }

    return RV_SUCCESS;
}

/* crea una nueva lista con los nodos de la lista original que al aplicarles el
 * filtro dan true */
retval_t LISTA_filtrar(lista_t plista, lista_t * pnueva, bool_t (*filtro)(void *)) {
    lista_t nueva;
    lista_t * pfinal;
    retval_t rv;

    if(plista == NULL || pnueva == NULL)
        return RV_ILLEGAL;

    if((rv = LISTA_crear(&nueva)) != RV_SUCCESS)
        return rv;

    pfinal = &nueva;
    while(plista != NULL) {
        if((*filtro)(plista->dato) == true) {
            if((rv = LISTA_insertar_al_final(pfinal, plista->dato)) != RV_SUCCESS) {
                LISTA_destruir(&nueva, NULL);
                return rv;
            }
            pfinal = &((*pfinal)->siguiente);
        }
        plista = plista->siguiente;
    }
    *pnueva = nueva;

    return RV_SUCCESS;
}
