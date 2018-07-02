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
#include "types.h"
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
 * dato al que apunta el nodo. Mientras la lista no esté vacía se guarda
 * el puntero al siguiente elemento de la lista y se destruye el nodo
 * actual con LISTA_destruir_nodo 
 * Devuelve un estado por el nombre y la lista actualizada por la interfaz */
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
 * de la lista. Se crea un nodo en el cual se guarda el puntero al dato.
 * Si la lista está vacía se apunta el puntero a la lista al nodo creado.
 * Si no lo está se itera avanzando por la lista hasta que el puntero al
 * siguiente nodo sea nulo, utilizando un puntero a nodo auxiliar, y se
 * guarda el nodo nuevo en el puntero al siguiente del último nodo de la
 * lista. Devuelve un estado por el nombre y la lista actualizada por la
 * interfaz */
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
/* Recibe un puntero a una lista previamente ordenada, un puntero a un
 * dato y una función que compara datos. se crea un nodo y se guarda el
 * dato en ese nodo. Si la lista está vacía o si la función de comparación
 * es menor a 0, se inserta el nodo al comienzo de la lista. De otra forma,
 * se itera guardando los punteros a los nodos actual y anterior hasta que
 * la función de comparación indique que el elemento a guardar es mayor al
 * siguiente elemento de la lista. Se termina por guardar el nodo creado
 * en el lugar del nodo actual, y elnodo actual a continuación del creado.
 * Devuelve la lista actualizada por la interfaz y un estado por el nombre */
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
/* Recibe un puntero a un nodo, un dato a buscar y un puntero a una función
 * que indica si dos elementos son iguales. Itera llamando a la función,
 * y si esta es distinta de 0, devuelve un puntero al dato encontrado. 
 * De no ser iguales apunta el puntero al siguiente nodo al puntero al
 * nodo actual.
 * De no encontrar el elemento, devuelve NULL */
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
/* Recibe un puntero a un nodo, el stream en donde imprimir y un puntero
 * a una función que imprime el tipo de dato guardado en la lista. Itera
 *  mientras imprime el dato llamando a la función pasada como argumento
 * y cambiando guardando el puntero al siguiente nodo en el puntero al nodo
 * actual.
 * Devuelve un estado por el nombre */
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
/* Recibe un puntero a un nodo, un puntero a una función la cual llevará
 * a cabo la operación deseada por el usuario y un puntero a un argumento
 * para dicha función. Se itera mientras no sea nulo el puntero al nodo,
 * llamando a la función pasada como argumento con el dato del nodo y el
 * argumento pasado como argumento. 
 * Devuelve un estado por el nombre */
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
/* Recibe una lista, un puntero a una lista nueva y una función la cual
 * indica si el dato actual cumple o no con determinada característica.
 * Crea una lista nueva e inserta al final de ella los elementos que hagan
 * que la función pasada como argumento devuelva true.
 * En caso de fallar destruye la lista creada.
 * Devuelve un estado por el nombre y la lista nueva por la interfaz */
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
