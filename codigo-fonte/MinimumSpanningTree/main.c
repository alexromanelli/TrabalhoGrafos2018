#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int origin;
    int destination;
    int weight;
} TipoAresta;

typedef struct TipoItemListaAresta {
    TipoAresta* item;
    struct TipoItemListaAresta* proximo;
} TipoItemListaAresta;

typedef struct {
    int num_itens;
    TipoItemListaAresta* primeiro;
} TipoListaAresta;

typedef struct {
    int n;
    int m;
    TipoListaAresta* listasIncidencias;
    TipoAresta** arestas; // um vetor de ponteiros para arestas, para referenciá-las fora das listas de adjacências
} TipoGrafo;

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

int read_line(FILE *in, char *buffer, size_t max) {
    return fgets(buffer, max, in) == buffer;
}

TipoGrafo* lerGrafoInstancia(char* nomeArquivo) {
    FILE *in;
    if((in = fopen(nomeArquivo, "rt")) != NULL) {
        TipoGrafo* grafo = (TipoGrafo*) malloc(sizeof(TipoGrafo));

        int c;
        char linha[200];
        int indArco;
        indArco = 0;
        while ((c = getc(in)) != EOF) {
            if (c == 'c') {
                read_line(in, linha, sizeof(linha));
            } else if (c == 'p') {
                fscanf(in, " %s %d %d", linha, &(grafo->n), &(grafo->m));
                grafo->m = grafo->m / 2; // considerar grafo não direcionado, então com a metade das arestas
                inicializarGrafo(grafo);
            } else if (c == 'a') {
                int origin, destination, w;
                fscanf(in, " %d %d %d", &origin, &destination, &w);
                if (!verificarAdjacente(grafo, origin - 1, destination - 1)) {
                    adicionarAresta(grafo, indArco++, origin - 1, destination - 1, w);
                }
            }
        }
        grafo->m = indArco; // ajusta valor m para quantidade real (arquivo de instância tem repetições)

        return grafo;
    }
    return NULL;
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

typedef struct TipoItemListaVertices {
    int vertice;
    struct TipoItemListaVertices* proximo;
} TipoItemListaVertices;

typedef struct {
    struct TipoItemListaVertices* inicioListaVertices;
} TipoConjunto;

typedef struct {
    int* indiceConjuntoVertice; // indica a qual conjunto cada vértice pertence
    TipoConjunto* conjuntos; // indica quais vértices pertencem a cada conjunto
} TipoConjuntosDisjuntos;

TipoConjuntosDisjuntos* inicializarConjuntosDisjuntos(int n) {
    TipoConjuntosDisjuntos* conjuntosDisjuntos = (TipoConjuntosDisjuntos*) malloc(sizeof(TipoConjuntosDisjuntos));
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

int acharConjunto(TipoConjuntosDisjuntos* conjuntosDisjuntos, int v) {
    return conjuntosDisjuntos->indiceConjuntoVertice[v];
}

/**
 * Faz a união dos conjuntos disjuntos a que pertencem os vértices u e v.
 */
void uniaoConjuntos(TipoConjuntosDisjuntos* conjuntosDisjuntos, int u, int v) {
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

/**
 * Encontra uma árvore geradora mínima (MST) com o algoritmo de Kruskal.
 * A saída é um array de ponteiros para arestas.
 **/
TipoAresta** kruskalMST(TipoGrafo* grafo, int* custo) {
    // ordenar arestas de G pelo valor de seus pesos (ordem crescente)
    ordenarArestas(grafo);

    // inicializar a saída T
    TipoAresta** T = (TipoAresta**) malloc((grafo->n - 1) * sizeof(TipoAresta*));
    int i;
    for (i = 0; i < grafo->n - 1; i++) {
        T[i] = NULL;
    }
    // inicializar estrutura para conjuntos disjuntos
    TipoConjuntosDisjuntos* conjuntosDisjuntos = inicializarConjuntosDisjuntos(grafo->n);

    (*custo) = 0;
    // percorrer as arestas de G, em ordem crescente, e incluir em T as que não forem acíclicas
    int indT = 0;
    for (i = 0; i < grafo->m; i++) {
        if (acharConjunto(conjuntosDisjuntos, grafo->arestas[i]->origin) !=
                acharConjunto(conjuntosDisjuntos, grafo->arestas[i]->destination)) {
            T[indT++] = grafo->arestas[i];
            uniaoConjuntos(conjuntosDisjuntos,
                    grafo->arestas[i]->origin,
                    grafo->arestas[i]->destination);
            (*custo) = (*custo) + grafo->arestas[i]->weight;
        }
    }

    return T;
}

void imprimirMST(TipoAresta** mst, int n) {
}

int main() {
    TipoGrafo* grafo = lerGrafoInstancia("USA-road-d.NY.gr");
    int custoMST = 0;
    TipoAresta** mst = kruskalMST(grafo, &custoMST);
    imprimirMST(mst, grafo->n);
    printf("\ncusto: %d\n", custoMST);
    limparMemoriaGrafo(grafo);
    return 0;
}
