
#include <stdlib.h>
#include "Grafo.h"
#include "ConjuntosDisjuntosLinkedLists.h"
#include "ConjuntosDisjuntosRootedTrees.h"

void inicializarGrafo(TipoGrafo* grafo) {
    grafo->listasIncidencias = (TipoListaAresta*) malloc(grafo->n * sizeof(TipoListaAresta));
    int i;
    for (i = 0; i < grafo->n; i++) {
        grafo->listasIncidencias[i].num_itens = 0;
        grafo->listasIncidencias[i].primeiro = NULL;
    }
    grafo->arestas = (TipoAresta**) malloc(grafo->m * sizeof(TipoAresta*));
}

void adicionarIncidente(TipoGrafo* grafo, int origin, TipoAresta* aresta) {
    if (grafo->listasIncidencias[origin].num_itens == 0) {
        grafo->listasIncidencias[origin].primeiro = (TipoItemListaAresta*) malloc(sizeof(TipoItemListaAresta));
        grafo->listasIncidencias[origin].primeiro->item = aresta;
        grafo->listasIncidencias[origin].primeiro->proximo = NULL;
    } else {
        struct TipoItemListaAresta* ultimoItem = grafo->listasIncidencias[origin].primeiro;
        while (ultimoItem->proximo != NULL) {
            ultimoItem = ultimoItem->proximo;
        }
        ultimoItem->proximo = (TipoItemListaAresta*) malloc(sizeof(TipoItemListaAresta));
        ultimoItem->proximo->item = aresta;
        ultimoItem->proximo->proximo = NULL;
    }
    grafo->listasIncidencias[origin].num_itens++;
}

void adicionarAresta(TipoGrafo* grafo, int indArco, int origin, int destination, int weight) {
    grafo->arestas[indArco] = (TipoAresta*) malloc(sizeof(TipoAresta));
    grafo->arestas[indArco]->origin = origin;
    grafo->arestas[indArco]->destination = destination;
    grafo->arestas[indArco]->weight = weight;

    adicionarIncidente(grafo, origin, grafo->arestas[indArco]);
    adicionarIncidente(grafo, destination, grafo->arestas[indArco]);
}

/**
 * Verificar se os vértices v1 e v2 são adjacentes. Se for, retornar 1, e se não for, 0.
 **/
int verificarAdjacente(TipoGrafo* grafo, int v1, int v2) {
    struct TipoItemListaAresta* itemLista = grafo->listasIncidencias[v1].primeiro;
    while (itemLista != NULL) {
        if (itemLista->item->origin == v2 || itemLista->item->destination == v2)
            return 1;
        itemLista = itemLista->proximo;
    }
    return 0;
}

void limparMemoriaGrafo(TipoGrafo* grafo) {
    int i;
    for (i = 0; i < grafo->n; i++) {
        while (grafo->listasIncidencias[i].primeiro != NULL) {
            struct TipoItemListaAresta* item = grafo->listasIncidencias[i].primeiro;
            grafo->listasIncidencias[i].primeiro = item->proximo;
            free(item);
        }
    }
    free(grafo->listasIncidencias);

    for (i = 0; i < grafo->m; i++) {
        free(grafo->arestas[i]);
    }
    free(grafo->arestas);
    free(grafo);
}

int particionar(TipoGrafo* grafo, int p, int r) {
    int pivot = grafo->arestas[r]->weight;
    int i = p - 1;
    int j;
    for (j = p; j < r; j++) {
        if (grafo->arestas[j]->weight <= pivot) {
            i = i + 1;
            // trocar elementos das posições i com j
            TipoAresta* elementoI = grafo->arestas[i];
            grafo->arestas[i] = grafo->arestas[j];
            grafo->arestas[j] = elementoI;
        }
    }
    // trocar elementos das posições i+1 com r
    TipoAresta* elementoR = grafo->arestas[r];
    grafo->arestas[r] = grafo->arestas[i + 1];
    grafo->arestas[i + 1] = elementoR;

    return i + 1;
}

void quickSort(TipoGrafo* grafo, int p, int r) {
    if (p < r) {
        int q = particionar(grafo, p, r);
        quickSort(grafo, p, q - 1);
        quickSort(grafo, q + 1, r);
    }
}
/**
 * Faz a ordenação do vetor grafo->arestas em ordem crescente de pesos.
 * Usa o algoritmo de ordenação Quick-Sort.
 **/
void ordenarArestas(TipoGrafo* grafo) {
    quickSort(grafo, 0, grafo->m - 1);
}

/**
 * Encontra uma árvore geradora mínima (MST) com o algoritmo de Kruskal.
 * A saída é um array de ponteiros para arestas.
 * Este método usa conjuntos disjuntos com listas encadeadas simples.
 **/
TipoAresta** kruskalMST_DSwLL(TipoGrafo* grafo, long* custo) {
    // ordenar arestas de G pelo valor de seus pesos (ordem crescente)
    ordenarArestas(grafo);

    // inicializar a saída T
    TipoAresta** T = (TipoAresta**) malloc((grafo->n - 1) * sizeof(TipoAresta*));
    int i;
    for (i = 0; i < grafo->n - 1; i++) {
        T[i] = NULL;
    }
    // inicializar estrutura para conjuntos disjuntos
    TipoConjuntosDisjuntosLL* conjuntosDisjuntos = inicializarConjuntosDisjuntosLL(grafo->n);

    (*custo) = 0;
    // percorrer as arestas de G, em ordem crescente, e incluir em T as que não forem acíclicas
    int indT = 0;
    for (i = 0; i < grafo->m; i++) {
        if (acharConjuntoLL(conjuntosDisjuntos, grafo->arestas[i]->origin) != 
                acharConjuntoLL(conjuntosDisjuntos, grafo->arestas[i]->destination)) {
            T[indT++] = grafo->arestas[i];
            uniaoConjuntosLL(conjuntosDisjuntos, 
                    grafo->arestas[i]->origin, 
                    grafo->arestas[i]->destination);
            (*custo) = (*custo) + grafo->arestas[i]->weight;
        }
    }
    
    limparMemoriaConjuntosDisjuntosLL(conjuntosDisjuntos);

    return T;
}

/**
 * Encontra uma árvore geradora mínima (MST) com o algoritmo de Kruskal.
 * A saída é um array de ponteiros para arestas.
 * Este método usa conjuntos disjuntos com árvores "enraizadas".
 **/
TipoAresta** kruskalMST_DSwRT(TipoGrafo* grafo, long* custo) {
    // ordenar arestas de G pelo valor de seus pesos (ordem crescente)
    ordenarArestas(grafo);

    // inicializar a saída T
    TipoAresta** T = (TipoAresta**) malloc((grafo->n - 1) * sizeof(TipoAresta*));
    int i;
    for (i = 0; i < grafo->n - 1; i++) {
        T[i] = NULL;
    }
    // inicializar estrutura para conjuntos disjuntos
    TipoConjuntosDisjuntosRT* conjuntosDisjuntos = inicializarConjuntosDisjuntosRT(grafo->n);

    (*custo) = 0;
    // percorrer as arestas de G, em ordem crescente, e incluir em T as que não forem acíclicas
    int indT = 0;
    for (i = 0; i < grafo->m; i++) {
        if (acharConjuntoRT(conjuntosDisjuntos, grafo->arestas[i]->origin) != 
                acharConjuntoRT(conjuntosDisjuntos, grafo->arestas[i]->destination)) {
            T[indT++] = grafo->arestas[i];
            uniaoConjuntosRT(conjuntosDisjuntos, 
                    grafo->arestas[i]->origin, 
                    grafo->arestas[i]->destination);
            (*custo) = (*custo) + grafo->arestas[i]->weight;
        }
    }
    
    limparMemoriaConjuntosDisjuntosRT(conjuntosDisjuntos);

    return T;
}

