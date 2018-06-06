/* 
 * File:   ConjuntosDisjuntosLinkedLists.h
 * Author: alex
 *
 * Created on 6 de Junho de 2018, 10:12
 * 
 * Declaração de conjuntos disjuntos com uso de listas encadeadas simples.
 */

#ifndef CONJUNTOSDISJUNTOSLINKEDLISTS_H
#define CONJUNTOSDISJUNTOSLINKEDLISTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

    typedef struct TipoItemListaVertices {
        int vertice;
        struct TipoItemListaVertices* proximo;
    } TipoItemListaVertices;

    typedef struct {
        struct TipoItemListaVertices* inicioListaVertices;
    } TipoConjunto;

    typedef struct {
        int n;
        int* indiceConjuntoVertice; // indica a qual conjunto cada vértice pertence
        TipoConjunto* conjuntos; // indica quais vértices pertencem a cada conjunto
    } TipoConjuntosDisjuntosLL;

    TipoConjuntosDisjuntosLL* inicializarConjuntosDisjuntosLL(int n);

    void limparMemoriaConjuntosDisjuntosLL(TipoConjuntosDisjuntosLL* conjuntosDisjuntos);

    int acharConjuntoLL(TipoConjuntosDisjuntosLL* conjuntosDisjuntos, int v);

    /**
     * Faz a união dos conjuntos disjuntos a que pertencem os vértices u e v.
     */
    void uniaoConjuntosLL(TipoConjuntosDisjuntosLL* conjuntosDisjuntos, int u, int v);

#ifdef __cplusplus
}
#endif

#endif /* CONJUNTOSDISJUNTOSLINKEDLISTS_H */

