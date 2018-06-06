
/* 
 * File:   Grafo.h
 * Author: alex
 *
 * Created on 6 de Junho de 2018, 10:20
 */

#ifndef GRAFO_H
#define GRAFO_H

#ifdef __cplusplus
extern "C" {
#endif

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

    void inicializarGrafo(TipoGrafo* grafo);

    void adicionarIncidente(TipoGrafo* grafo, int origin, TipoAresta* aresta);

    void adicionarAresta(TipoGrafo* grafo, int indArco, int origin, int destination, int weight);

    /**
     * Verificar se os vértices v1 e v2 são adjacentes. Se for, retornar 1, e se não for, 0.
     **/
    int verificarAdjacente(TipoGrafo* grafo, int v1, int v2);

    void limparMemoriaGrafo(TipoGrafo* grafo);

    /**
     * Encontra uma árvore geradora mínima (MST) com o algoritmo de Kruskal.
     * A saída é um array de ponteiros para arestas.
     * Este método usa conjuntos disjuntos com listas encadeadas simples.
     **/
    TipoAresta** kruskalMST_DSwLL(TipoGrafo* grafo, long* custo);

    /**
     * Encontra uma árvore geradora mínima (MST) com o algoritmo de Kruskal.
     * A saída é um array de ponteiros para arestas.
     * Este método usa conjuntos disjuntos com árvores "enraizadas".
     **/
    TipoAresta** kruskalMST_DSwRT(TipoGrafo* grafo, long* custo);


#ifdef __cplusplus
}
#endif

#endif /* GRAFO_H */

