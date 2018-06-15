/*
 * Essas estruturas e métodos foram usados para testar as soluções encontradas.
 */

/* 
 * File:   GrafoPF.h
 * Author: Paulo Feofiloff
 * Adaptado de: https://www.ime.usp.br/~pf/algoritmos_para_grafos/
 */

#include <stdlib.h>
#include <stdbool.h>
#include "GrafoPF.h"

/* A função NEWnode() recebe um vértice w e o endereço next de um nó e devolve o endereço a de um novo nó tal que a->w == w e a->next == next. */

static link NEWnode( vertex w, link next) { 
   link a = malloc( sizeof (struct node));
   a->w = w; 
   a->next = next;     
   return a;                         
}

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: A função GRAPHinit() constrói um grafo com vértices 0 1 .. V-1 e nenhum arco. */

Graph GRAPHinit( int V) { 
   Graph G = malloc( sizeof *G);
   G->V = V; 
   G->A = 0;
   G->adj = malloc( V * sizeof (link));
   for (vertex v = 0; v < V; ++v) 
      G->adj[v] = NULL;
   return G;
}

/* REPRESENTAÇÃO POR LISTAS DE ADJACÊNCIA: A função GRAPHinsertArc() insere um arco v-w no grafo G. A função supõe que v e w são distintos, positivos e menores que G->V. Se o grafo já tem um arco v-w, a função não faz nada. */

void GRAPHinsertArc( Graph G, vertex v, vertex w) { 
   for (link a = G->adj[v]; a != NULL; a = a->next) 
      if (a->w == w) return;
   G->adj[v] = NEWnode( w, G->adj[v]);
   G->A++;
}

/* A função circuitR() devolve true se encontrar um circuito ao percorrer G a partir do vértice v. */

static bool circuitR( UGraph G, vertex v, int *cnt) 
{ 
   pre[v] = (*cnt)++;
   for (link a = G->adj[v]; a != NULL; a = a->next) {
      vertex w = a->w;
      if (pre[w] == -1) {
         pa[w] = v; 
         if (circuitR( G, w, cnt))
            return true;
      } else { /* v-w é arco de retorno */
         if (w != pa[v]) /* não-trivial */
            return true; 
      }
   }
   return false;
}

/* A função UGRAPHhasCircuit() decide se o grafo não-dirigido G tem um circuito. */

bool UGRAPHhasCircuit( UGraph G, int *cnt) 
{ 
   (*cnt) = 0;
   for (vertex v = 0; v < G->V; ++v)
      pre[v] = -1;
   for (vertex v = 0; v < G->V; ++v)
      if (pre[v] == -1) {
         pa[v] = v; /* v é uma raiz */
         if (circuitR( G, v, cnt)) return true;
      }
   return false;
}

