#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Grafo.h"
#include "ConjuntosDisjuntosLinkedLists.h"
#include "ConjuntosDisjuntosRootedTrees.h"

#include "GrafoPF.h"

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
                if (fscanf(in, " %s %d %d", linha, &(grafo->n), &(grafo->m)) != EOF) {
                    grafo->m = grafo->m; // considerar grafo não direcionado, então com a metade das arestas
                    inicializarGrafo(grafo);
                } else {
                    printf("ERRO DE LEITURA DO ARQUIVO.");
                    return NULL;
                }
            } else if (c == 'a') {
                int origin, destination, w;
                if (fscanf(in, " %d %d %d", &origin, &destination, &w) != EOF) {
                    adicionarAresta(grafo, indArco++, origin - 1, destination - 1, w);
                } else {
                    printf("ERRO DE LEITURA DO ARQUIVO.");
                    limparMemoriaGrafo(grafo);
                    return NULL;
                }
            }
        }
        fclose(in);
        grafo->m = indArco; // ajusta valor m para quantidade real (arquivo de instância tem repetições)
        
        return grafo;
    }
    return NULL;
}

int mstContemAresta(TipoAresta** mst, int numArestasMST, TipoAresta* aresta) {
    int i;
    for (i = 0; i < numArestasMST; i++) {
        if (mst[i] == aresta)
            return 1;
    }
    return 0;
}

typedef struct {
    int x;
    int y;
} Ponto;

/**
 * Constrói arquivo de saída com a solução encontrada. Usado para fazer a visualização da árvore.
 * @param grafo Grafo de entrada, que é a instância do problema de MST.
 * @param mst A árvore geradora mínima encontrada. É a solução para a instância.
 * @param nomeArquivoInstancia O nome do arquivo que contém a instância solucionada.
 */
void imprimirMST(TipoGrafo* grafo, TipoAresta** mst, char* nomeArquivoInstancia) {
    char nomeArqSaida[100] = "";
    strncat(nomeArqSaida, nomeArquivoInstancia, 100);
    strncat(nomeArqSaida, ".mst", 100);
    FILE* out;
    if ((out = fopen(nomeArqSaida, "w")) != NULL) {
        fprintf(out, "%d\n", grafo->n);
        int i;
        for (i = 0; i < grafo->n - 1; i++) {
            fprintf(out, "%d %d\n", mst[i]->origin, mst[i]->destination);
        }
        fclose(out);
    }
}

int ehArvore(TipoGrafo* grafo, TipoAresta** mst) {
    Graph g = GRAPHinit(grafo->n);
    int i;
    for (i = 0; i < grafo->n - 1; i++) {
        GRAPHinsertArc(g, mst[i]->origin, mst[i]->destination);
        GRAPHinsertArc(g, mst[i]->destination, mst[i]->origin);
    }
    int cnt;
    if (UGRAPHhasCircuit(g, &cnt)) {
        return 0;
    }
    return 1;
}

int testarMST(TipoGrafo* grafo, TipoAresta** mst) {
    printf("Verificação de árvore geradora mínima:\n");

    // verificar se é conexa e abrange todos os vértices
    printf("  --> abrangência...");
    int* verticeAlcancado = (int*) malloc(grafo->n * sizeof(int));
    int i;
    for (i = 0; i < grafo->n; i++) {
        verticeAlcancado[i] = 0;
    }
    for (i = 0; i < grafo->n - 1; i++) {
        int v1 = mst[i]->origin;
        int v2 = mst[i]->destination;
        verticeAlcancado[v1] = 1;
        verticeAlcancado[v2] = 1;
    }
    int somaVerticesAlcancados = 0;
    for (i = 0; i < grafo->n; i++) {
        somaVerticesAlcancados += verticeAlcancado[i];
    }
    free(verticeAlcancado);
    if (somaVerticesAlcancados == grafo->n) {
        printf("OK\n");
    } else {
        printf("FALHOU!\n");
        return 0;
    }
    
    // verificar se é acíclica
    printf("  --> acíclica...");
    if (ehArvore(grafo, mst)) {
        printf("OK\n");
    } else {
        printf("FALHOU!\n");
        return 0;
    }
    
    return 1;
}

int main() {
    int numInstancias = 8;
    char* nomesArquivosInstancias[8] = {
            "USA-road-d.NY.gr", "USA-road-d.FLA.gr", "USA-road-d.COL.gr", "USA-road-d.BAY.gr",
            "USA-road-t.NY.gr", "USA-road-t.FLA.gr", "USA-road-t.COL.gr", "USA-road-t.BAY.gr"
    };

    printf("Instância;|V|;|E|;Estrutura;Custo;Tempo de execução\n");
    int i;
    for (i = 0; i < numInstancias; i++) {
        char nomeArquivo[100] = "instancias/";
        strncat(nomeArquivo, nomesArquivosInstancias[i], 100);
        
        TipoGrafo* grafo = lerGrafoInstancia(nomeArquivo);
        if (grafo == NULL) {
            continue;
        }
        long custoMST = 0;
        clock_t startClock = clock();
        TipoAresta** mst = kruskalMST_DSwRT(grafo, &custoMST);
        clock_t endClock = clock();
        double totalTime = (double)(endClock - startClock) / CLOCKS_PER_SEC;
        printf("%s;%d;%d;Rooted Trees;%ld;%f\n", nomesArquivosInstancias[i], 
                grafo->n, grafo->m, custoMST, totalTime);
//        imprimirMST(grafo, mst, nomesArquivosInstancias[i]);
//        if (testarMST(grafo, mst)) {
//            printf("Árvore geradora válida.\n");
//        } else {
//            printf("Árvore geradora inválida.\n");
//        }
        free(mst);
        limparMemoriaGrafo(grafo);

        grafo = lerGrafoInstancia(nomeArquivo);
        if (grafo == NULL) {
            continue;
        }
        custoMST = 0;
        startClock = clock();
        mst = kruskalMST_DSwLL(grafo, &custoMST);
        endClock = clock();
        totalTime = (double)(endClock - startClock) / CLOCKS_PER_SEC;
        printf("%s;%d;%d;Linked Lists;%ld;%f\n", nomesArquivosInstancias[i], 
                grafo->n, grafo->m, custoMST, totalTime);
//        if (testarMST(grafo, mst)) {
//            printf("Árvore geradora válida.\n");
//        } else {
//            printf("Árvore geradora inválida.\n");
//        }
        free(mst);
        limparMemoriaGrafo(grafo);
    }
    return 0;
}
