/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConjuntosDisjuntosRootedTrees.h
 * Author: alex
 *
 * Created on 6 de Junho de 2018, 10:33
 */

#ifndef CONJUNTOSDISJUNTOSROOTEDTREES_H
#define CONJUNTOSDISJUNTOSROOTEDTREES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

    typedef struct TipoNodeRootedTree {
        int vertice; // o vértice referenciado por este nó da árvore
        int limiteSuperiorAltura; // a altura do nó, considerada um limite superior por não ser garantida sua atualização durante as compactações
        struct TipoNodeRootedTree* pai; // o nó pai deste nó (se for a raiz, o pai será o próprio nó)
    } TipoNodeRootedTree;
    
    typedef struct {
        int n;
        struct TipoNodeRootedTree** arrayReferenciaVerticeNo; // faz a correspondência entre um vértice e o nó que o referencia
    } TipoConjuntosDisjuntosRT;

    TipoConjuntosDisjuntosRT* inicializarConjuntosDisjuntosRT(int n);

    void limparMemoriaConjuntosDisjuntosRT(TipoConjuntosDisjuntosRT* conjuntosDisjuntos);

    int acharConjuntoRT(TipoConjuntosDisjuntosRT* conjuntosDisjuntos, int v);

    /**
     * Faz a união dos conjuntos disjuntos a que pertencem os vértices u e v.
     */
    void uniaoConjuntosRT(TipoConjuntosDisjuntosRT* conjuntosDisjuntos, int u, int v);

#ifdef __cplusplus
}
#endif

#endif /* CONJUNTOSDISJUNTOSROOTEDTREES_H */

