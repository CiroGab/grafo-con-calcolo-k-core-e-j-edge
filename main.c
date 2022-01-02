#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc!=2)
        exit(1);

    FILE *fp=fopen(argv[1], "r");
    Graph G;
    int j,k;

    G=GRAPHload(fp);
    printf("Inserire k: \n");
    scanf("%d", &k);
    if(k<0)
        fprintf(stderr, "Errore!");
    else
        GRAPHkcore(G, k);
    printf("Inserire j: \n");
    scanf("%d", &j);
    if(j<1)
        fprintf(stderr, "Errore!");
    else
        GRAPHjeconnected(G, j);
    GRAPHfree(G);
    return 0;

}
