/**
 * Implementação de conjuntos disjuntos com listas encadeadas simples.
 **/

#include "ConjuntosDisjuntosLinkedLists.h"

TipoConjuntosDisjuntosLL* inicializarConjuntosDisjuntosLL(int n) {
    TipoConjuntosDisjuntosLL* conjuntosDisjuntos = (TipoConjuntosDisjuntosLL*) malloc(sizeof(TipoConjuntosDisjuntosLL));
    conjuntosDisjuntos->n = n;
    conjuntosDisjuntos->indiceConjuntoVertice = (int*) malloc(n * sizeof(int));
    conjuntosDisjuntos->conjuntos = (TipoConjunto*) malloc(n * sizeof(TipoConjunto));
    int i;
    for (i = 0; i < n; i++) {
        conjuntosDisjuntos->indiceConjuntoVertice[i] = i;
        conjuntosDisjuntos->conjuntos[i].inicioListaVertices = (TipoItemListaVertices*) malloc(sizeof(TipoItemListaVertices));
        conjuntosDisjuntos->conjuntos[i].inicioListaVertices->vertice = i;
        conjuntosDisjuntos->conjuntos[i].inicioListaVertices->proximo = NULL;
    }
    return conjuntosDisjuntos;
}

void limparMemoriaConjuntosDisjuntosLL(TipoConjuntosDisjuntosLL* conjuntosDisjuntos) {
    free(conjuntosDisjuntos->indiceConjuntoVertice);
    int i;
    for (i = 0; i < conjuntosDisjuntos->n; i++) {
        struct TipoItemListaVertices* itemVertice = conjuntosDisjuntos->conjuntos[i].inicioListaVertices;
        while (itemVertice != NULL) {
            conjuntosDisjuntos->conjuntos[i].inicioListaVertices = itemVertice->proximo;
            free(itemVertice);
            itemVertice = conjuntosDisjuntos->conjuntos[i].inicioListaVertices;
        }
    }
    free(conjuntosDisjuntos->conjuntos);
}

int acharConjuntoLL(TipoConjuntosDisjuntosLL* conjuntosDisjuntos, int v) {
    return conjuntosDisjuntos->indiceConjuntoVertice[v];
}

/**
 * Faz a união dos conjuntos disjuntos a que pertencem os vértices u e v.
 */
void uniaoConjuntosLL(TipoConjuntosDisjuntosLL* conjuntosDisjuntos, int u, int v) {
    int indConjuntoDeU = conjuntosDisjuntos->indiceConjuntoVertice[u];
    int indConjuntoDeV = conjuntosDisjuntos->indiceConjuntoVertice[v];
    // percorrer vértices do conjunto de v, alterando seu conjunto para ser o conjunto de u
    struct TipoItemListaVertices* itemVertice = conjuntosDisjuntos->conjuntos[indConjuntoDeV].inicioListaVertices;
    while (itemVertice != NULL) {
        int vertice = itemVertice->vertice;
        conjuntosDisjuntos->indiceConjuntoVertice[vertice] = indConjuntoDeU;
        itemVertice = itemVertice->proximo;
    }
    // incluir lista de vértices do conjunto de v no final da lista de vértices do conjunto de u
    itemVertice = conjuntosDisjuntos->conjuntos[indConjuntoDeU].inicioListaVertices;
    while (itemVertice->proximo != NULL) {
        itemVertice = itemVertice->proximo;
    }
    itemVertice->proximo = conjuntosDisjuntos->conjuntos[indConjuntoDeV].inicioListaVertices;
    // eliminar conjunto de v
    conjuntosDisjuntos->conjuntos[indConjuntoDeV].inicioListaVertices = NULL;
}
