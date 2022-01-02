#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct graph *Graph;
typedef struct{
    int v, w;
}Edge;

Graph GRAPHload(FILE* fin);
Graph GRAPHinit(int V);
void GRAPHedges(Graph G, Edge *a);
void GRAPHinsertE(Graph G, int id1, int id2);
void GRAPHkcore(Graph G, int k);
void GRAPHjeconnected(Graph G, int j);
void rimettiVertex(Graph G);
void GRAPHfree(Graph G);
void GRAPHremoveE(Graph G, Edge e);

#endif
