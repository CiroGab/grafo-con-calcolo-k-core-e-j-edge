#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "ST.h"

typedef struct node *link;
struct node{
    int v;
    int preso;
    link next;
};

struct graph{
    int V,E;
    link *ladj;
    link z;
    ST tab;
};

link NEW(int v, link next){
    link t=malloc(sizeof(*t));
    t->v=v;
    t->next=next;
    t->preso=1;
    return t;
}

void GRAPHfree(Graph G){
    int v;
    link t, next;
    for(v=0; v<G->V; v++)
        for(t=G->ladj[v]; t!=G->z; t=next){
            next=t->next;
            free(t);
        }
    STfree(G->tab);
    free(G->ladj);
    free(G->z);
    free(G);
}

Graph GRAPHinit(int V){
    int v;
    Graph G=malloc(sizeof(*G));
    G->V=V;
    G->E=0;
    G->z=NEW(-1, NULL);
    G->ladj=malloc(G->V* sizeof(link));
    for(v=0; v<G->V; v++)
        G->ladj[v]=G->z;
    G->tab=STinit(V);
    return G;
}

static Edge EDGEcreate(int v, int w){
    Edge e;
    e.v=v; e.w=w;
    return e;
}

Graph GRAPHload(FILE *fin){
    int V, i, id1, id2;
    Item label1, label2;
    Graph G;
    fscanf(fin, "%d", &V);
    G=GRAPHinit(V);
    for(i=0; i<G->V; i++){
        fscanf(fin, "%s", label1.nome);
        STinsert(G->tab, label1);
    }
    while(fscanf(fin, "%s %s", label1.nome, label2.nome)==2){
        id1=STsearch(G->tab, label1.nome);
        id2=STsearch(G->tab, label2.nome);
        if(id1>=0 && id2>=0)
            GRAPHinsertE(G, id1, id2);
    }
    return G;
}

void GRAPHedges(Graph G, Edge *a){
    int v,E=0;
    link t;
    for(v=0; v<G->V; v++)
        for(t=G->ladj[v]; t!=G->z; t=t->next)
            a[E++]=EDGEcreate(v, t->v);
}

static void insertE(Graph G, Edge e){
    int v=e.v, w=e.w;
    G->ladj[v]=NEW(w, G->ladj[v]);
    G->ladj[w]=NEW(v, G->ladj[w]);
    G->E++;
}

void GRAPHinsertE(Graph G, int id1, int id2){
    insertE(G, EDGEcreate(id1, id2));
}


void calcolaDeg(Graph G, int k, int *deg){
    link t;
    int i;

    for(i=0; i<G->V; i++)
        deg[i]=0;

    for(i=0; i<G->V; i++){
        for (t = G->ladj[i]; t != G->z; t = t->next) {
            if (G->ladj[t->v]->preso == 1 && G->ladj[i]->preso == 1) {
                deg[i]++;
            }
        }
    }

    for(i=0; i<G->V; i++){
        if(deg[i]<k && G->ladj[i]->preso==1){
            G->ladj[i]->preso=0;
            calcolaDeg(G, k, deg);
        }
    }

}

void rimettiVertex(Graph G){
    int i;
    for(i=0; i<G->V; i++){
        G->ladj[i]->preso=1;
    }
}

void GRAPHkcore(Graph G, int k){
    int *sol, i;
    sol=malloc(G->V* sizeof(int));
    calcolaDeg(G, k, sol);
    printf("Vertici %d-core:\n", k);
    for(i=0; i<G->V; i++)
        if(sol[i]!=0)
            printf("%s\n", STsearchByIndex(G->tab, i));
    rimettiVertex(G);
}


void dfsRcc(Graph G, int v, int id, int *cc){
    link t;
    cc[v]=id;
    for(t=G->ladj[v]; t!=G->z; t=t->next)
        if(cc[t->v]==-1)
            dfsRcc(G, t->v, id, cc);
}

int GRAPHcc(Graph G){
    int v, id=0, *cc;
    cc=malloc(G->V* sizeof(int));
    for(v=0; v<G->V; v++) cc[v]=-1;
    for(v=0; v<G->V; v++)
        if(cc[v]==-1)
            dfsRcc(G, v, id++, cc);

    return id;
}

int powersetR(Edge *val, int j, int i, int pos, int start, Graph G, int *sol){
    int c, ris, k;
    if(pos==i){
        int E=2*G->E;
        for(k=0; k<E; k++){
            if(sol[k]==1) {
                GRAPHremoveE(G, val[k]);
            }
        }

        if(GRAPHcc(G)>1) {
            if (j == i) {
                printf("Il grafo e' %d-connected. Archi che rendono il grafo non connesso:\n", j);
                for (k = 0; k < E; k++) {
                    if (sol[k] == 1) {
                        printf("%s-%s\n", STsearchByIndex(G->tab, val[k].v), STsearchByIndex(G->tab, val[k].w));
                        GRAPHinsertE(G, val[k].v, val[k].w);
                    }
                }
                return 1;
            } else {
                printf("Il grafo non e' %d-connected.\n", j);
                for (k = 0; k < E; k++) {
                    if (sol[k] == 1) {
                        GRAPHinsertE(G, val[k].v, val[k].w);
                    }
                }
                return 0;
            }
        }
        for(k=0; k<E; k++){
            if(sol[k]==1)
                GRAPHinsertE(G, val[k].v, val[k].w);
        }

        return 2;
    }

    for(c=start; c<2*G->E; c++){
        if(sol[c]==0) {
            sol[c]=1;
            ris = powersetR(val, j, i, pos + 1, c + 1, G, sol);
            sol[c]=0;
            if(ris==0 || ris==1) {
                return ris;
            }
        }

    }
    return 2;
}

void powerset(Edge *val, int j, Graph G){
    int i, ris, *sol;
    for(i=1; i<=j; i++){
        sol=calloc(2*G->E, sizeof(int));
        ris=powersetR(val, j, i, 0, 0, G, sol);
        free(sol);
        if(ris==1 || ris==0)
            return;
    }
    if(ris==2)
        printf("Il grafo non e' %d-edge-connected.\n", j);
}

static void removeE(Graph G, Edge e){
    int v=e.v, w=e.w;
    link x;
    if(G->ladj[v]->v==w){
        G->ladj[v]=G->ladj[v]->next;
        G->E--;
    } else{
        for(x=G->ladj[v]; x!=G->z; x=x->next)
            if(x->next->v==w) {
                x->next = x->next->next;
                G->E--;
            }
    }

    if (G->ladj[w]->v == v) {
        G->ladj[w] = G->ladj[w]->next;
    }
    else {
        for (x = G->ladj[w]; x != G->z; x = x->next)
            if (x->next->v == v)
                x->next = x->next->next;
    }
}

void GRAPHremoveE(Graph G, Edge e){
    removeE(G, e);
}
void GRAPHjeconnected(Graph G, int j) {
    Edge *a;
    a = malloc(2 * G->E * sizeof(Edge));
    GRAPHedges(G, a);
    powerset(a, j, G);
}
