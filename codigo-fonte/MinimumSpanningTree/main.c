#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Grafo.h"
#include "ConjuntosDisjuntosLinkedLists.h"
#include "ConjuntosDisjuntosRootedTrees.h"

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

void imprimirMST(TipoAresta** mst, int n) {
}

int main() {
    int numInstancias = 8;
    char* nomesArquivosInstancias[8] = {
            "USA-road-d.NY.gr", "USA-road-d.FLA.gr", "USA-road-d.COL.gr", "USA-road-d.BAY.gr",
            "USA-road-t.NY.gr", "USA-road-t.FLA.gr", "USA-road-t.COL.gr", "USA-road-t.BAY.gr"
    };

    printf("Instância\tEstrutura\tCusto\tTempo de execução\n");
    int i;
    for (i = 0; i < numInstancias; i++) {
        char nomeArquivo[100] = "instancias/";
        strcat(nomeArquivo, nomesArquivosInstancias[i]);
        
        TipoGrafo* grafo = lerGrafoInstancia(nomeArquivo);
        long custoMST = 0;
        clock_t startClock = clock();
        TipoAresta** mst = kruskalMST_DSwLL(grafo, &custoMST);
        clock_t endClock = clock();
        double totalTime = (double)(endClock - startClock) / CLOCKS_PER_SEC;
        printf("%s\tLinked Lists\t%ld\t%f\n", nomesArquivosInstancias[i], custoMST, totalTime);
        limparMemoriaGrafo(grafo);

        grafo = lerGrafoInstancia(nomeArquivo);
        custoMST = 0;
        startClock = clock();
        mst = kruskalMST_DSwRT(grafo, &custoMST);
        endClock = clock();
        totalTime = (double)(endClock - startClock) / CLOCKS_PER_SEC;
        printf("%s\tRooted Trees\t%ld\t%f\n", nomesArquivosInstancias[i], custoMST, totalTime);
        limparMemoriaGrafo(grafo);
    }
    return 0;
}
