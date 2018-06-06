
#include <stdlib.h>
#include "ConjuntosDisjuntosRootedTrees.h"

TipoConjuntosDisjuntosRT* inicializarConjuntosDisjuntosRT(int n) {
    TipoConjuntosDisjuntosRT* conjuntosDisjuntos = (TipoConjuntosDisjuntosRT*)
            malloc(sizeof (TipoConjuntosDisjuntosRT));

    conjuntosDisjuntos->n = n;
    conjuntosDisjuntos->arrayReferenciaVerticeNo = (TipoNodeRootedTree**)
            malloc(n * sizeof (TipoNodeRootedTree*));
    int i;
    for (i = 0; i < n; i++) {
        conjuntosDisjuntos->arrayReferenciaVerticeNo[i] = (TipoNodeRootedTree*)
                malloc(sizeof(TipoNodeRootedTree));
        conjuntosDisjuntos->arrayReferenciaVerticeNo[i]->vertice = i;
        conjuntosDisjuntos->arrayReferenciaVerticeNo[i]->limiteSuperiorAltura = 1;
        conjuntosDisjuntos->arrayReferenciaVerticeNo[i]->pai = conjuntosDisjuntos->arrayReferenciaVerticeNo[i];
    }

    return conjuntosDisjuntos;
}

void limparMemoriaConjuntosDisjuntosRT(TipoConjuntosDisjuntosRT* conjuntosDisjuntos) {
    int i;
    for (i = 0; i < conjuntosDisjuntos->n; i++) {
        free(conjuntosDisjuntos->arrayReferenciaVerticeNo[i]);
    }
    free(conjuntosDisjuntos->arrayReferenciaVerticeNo);
    free(conjuntosDisjuntos);
}

int acharConjuntoRT(TipoConjuntosDisjuntosRT* conjuntosDisjuntos, int v) {
    TipoNodeRootedTree* node = conjuntosDisjuntos->arrayReferenciaVerticeNo[v];
    while (node->pai != node) { // quando sair do loop, node será a raiz
        node = node->pai;
    }
    int verticeRaiz = node->vertice;
    
    // compactação de árvore (faz nós ao longo do caminho serem filhos da raiz)
    TipoNodeRootedTree* raiz = node;
    node = conjuntosDisjuntos->arrayReferenciaVerticeNo[v];
    while (node->pai != node) {
        TipoNodeRootedTree* pai = node->pai;
        node->pai = raiz;
        node = pai;
    }
    
    return verticeRaiz;
}

/**
 * Faz a união dos conjuntos disjuntos a que pertencem os vértices u e v.
 */
void uniaoConjuntosRT(TipoConjuntosDisjuntosRT* conjuntosDisjuntos, int u, int v) {
    int verticeRaizConjuntoDeU = acharConjuntoRT(conjuntosDisjuntos, u);
    int verticeRaizConjuntoDeV = acharConjuntoRT(conjuntosDisjuntos, v);
    
    TipoNodeRootedTree* raizConjuntoDeU = conjuntosDisjuntos->arrayReferenciaVerticeNo[verticeRaizConjuntoDeU];
    TipoNodeRootedTree* raizConjuntoDeV = conjuntosDisjuntos->arrayReferenciaVerticeNo[verticeRaizConjuntoDeV];
    
    if (raizConjuntoDeU->limiteSuperiorAltura >= raizConjuntoDeV->limiteSuperiorAltura) {
        raizConjuntoDeV->pai = raizConjuntoDeU;
        if (raizConjuntoDeV->limiteSuperiorAltura + 1 > raizConjuntoDeU->limiteSuperiorAltura)
            raizConjuntoDeU->limiteSuperiorAltura = raizConjuntoDeV->limiteSuperiorAltura + 1;
    } else {
        raizConjuntoDeU->pai = raizConjuntoDeV;
        if (raizConjuntoDeU->limiteSuperiorAltura + 1 > raizConjuntoDeV->limiteSuperiorAltura)
            raizConjuntoDeV->limiteSuperiorAltura = raizConjuntoDeU->limiteSuperiorAltura + 1;
    }
}

