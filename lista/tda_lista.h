/**
 * @file tda_lista.h
 * @brief Encabezado y definición de primitivas para una lista polimórfica
 * simplemente enlazada. Las primitivas implementadas pueden ser recursivas o
 * iterativas, según el archivo .c contra el que se compile.
 *
 * En esta implementación de lista polimórfica simplemente enlazada, la lista
 * no es dueña del dato que almacena. Si lo fuese, debería recibir, en cada
 * función que crea nuevos nodos, un puntero a función que sepa cómo clonar el
 * dato apuntado, de forma de almacenar una copia del dato.
 *
 * IMPORTANTE: La función LISTA_filtrar() retorna una lista con los datos sin
 * replicar, por lo mencionado anteriormente.
 *
 * Para salvar eso, LISTA_crear_nodo(), LISTA_insertar_XXX() y LISTA_filtrar()
 * deberían recibir el clonador de datos.
 *
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
#ifndef TDA_LISTA__H
#define TDA_LISTA__H

#include <stdio.h>
#include "../lib/types.h"
/* en types.h están definidos los tipos:
typedef enum retval {
  RV_SUCCESS = 0,
  RV_ILLEGAL = 1,
  RV_NOSPACE = 2,
  RV_ERROR = 3,
  RV_NOTIMPLEMENTED = 4,
  RV_MISSING_ARGS = 5
} retval_t;

typedef enum bool {
  false = 0,
  true = 1
} bool_t;

¿conviene utilizar lst_retval_t en lugar de retval_t?
*/

typedef struct nodo {
  struct nodo * siguiente;
  void * dato;
} nodo_t, * lista_t;

bool_t LISTA_esta_vacia(lista_t);
retval_t LISTA_crear(lista_t *);
retval_t LISTA_crear_nodo(nodo_t ** , void *);
retval_t LISTA_destruir_nodo(nodo_t **, retval_t (*destructor_dato)(void *));
retval_t LISTA_destruir(lista_t *, retval_t (*destructor_dato)(void *));
retval_t LISTA_insertar_al_ppio(lista_t *, void *);
retval_t LISTA_insertar_al_final(lista_t *, void *);
retval_t LISTA_insertar_decreciente(lista_t *, void *, int (*cmp)(void *, void *));

void * LISTA_buscar(nodo_t *, void *, int (*cmp)(void *, void *));
retval_t LISTA_imprimir(lista_t, FILE *, retval_t (*impresor)(void *, FILE *));
retval_t LISTA_recorrer(lista_t, retval_t (*pf)(void *, void *), void *);
retval_t LISTA_filtrar(lista_t, lista_t *, bool_t (*filtro)(void *));

#endif
