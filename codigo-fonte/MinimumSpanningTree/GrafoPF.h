/*
 * Essas estruturas e métodos foram usados para testar as soluções encontradas.
 */

/* 
 * File:   GrafoPF.h
 * Author: Paulo Feofiloff
 * Adaptado de: https://www.ime.usp.br/~pf/algoritmos_para_grafos/
 */

#ifndef GRAFOPF_H
#define GRAFOPF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Vértices de grafos são representados por objetos do tipo vertex. */

#define vertex int
    
#define UGraph Graph

/* A lista de adjacência de um vértice v é composta por nós do tipo node. Cada nó da lista corresponde a um arco e contém um vizinho w de v e o endereço do nó seguinte da lista. Um link é um ponteiro para um node. */

typedef struct node *link;
struct node { 
   vertex w; 
   link next; 
};

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: A estrutura graph representa um grafo. O campo adj é um ponteiro para o vetor de listas de adjacência, o campo V contém o número de vértices e o campo A contém o número de arcos do grafo. */

struct graph {
   int V; 
   int A; 
   link *adj; 
};
/* Um Graph é um ponteiro para um graph. */

typedef struct graph *Graph;
/* A função NEWnode() recebe um vértice w e o endereço next de um nó e devolve o endereço a de um novo nó tal que a->w == w e a->next == next. */

static link NEWnode( vertex w, link next);

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: A função GRAPHinit() constrói um grafo com vértices 0 1 .. V-1 e nenhum arco. */

Graph GRAPHinit( int V);

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: A função GRAPHinsertArc() insere um arco v-w no grafo G. A função supõe que v e w são distintos, positivos e menores que G->V. Se o grafo já tem um arco v-w, a função não faz nada. */

void GRAPHinsertArc( Graph G, vertex v, vertex w);

static int pre[1100000];
static vertex pa[1100000];


/* A função circuitR() devolve true se encontrar um circuito ao percorrer G a partir do vértice v. */

static bool circuitR( UGraph G, vertex v, int *cnt);

/* A função UGRAPHhasCircuit() decide se o grafo não-dirigido G tem um circuito. */

bool UGRAPHhasCircuit( UGraph G, int *cnt);

#ifdef __cplusplus
}
#endif

#endif /* GRAFOPF_H */

