#include "ST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbtab{
    Item *tab;
    int size;
    int maxN;
};

Item ITEMsetNULL(){
    Item i={""};
    return i;
}

ST STinit(int maxN){
    ST st; int i;
    st=malloc(sizeof(*st));
    st->tab=malloc(maxN* sizeof(Item));
    for(i=0; i<maxN; i++){
        st->tab[i]=ITEMsetNULL();
    }
    st->maxN=maxN;
    st->size=0;
    return st;
}

int STcount(ST st){
    return st->size;
}

void STfree(ST st){
    free(st->tab);
    free(st);
}

void STinsert(ST st, Item val) {
    int j, i;

    for(j=0; j<st->size; j++)
        if(strcmp(val.nome, st->tab[j].nome)==0)
            return;

    i=st->size++;
    if (st->size > st->maxN) {
        st->tab = realloc(st->tab, (2 * st->maxN) * sizeof(Item));
        if (st->tab == NULL) return;
        st->maxN = 2 * st->maxN;
    }
    //Il nuovo elemento viene inserito nella sua posizione in ordine alfabetico
    while ((i > 0) && (strcmp(val.nome, st->tab[i-1].nome)<0) ) {
        st->tab[i] = st->tab[i - 1];
        i--;
    }

    st->tab[i] = val;
}

static int searchR(ST st, int l, int r, char * label){
    int m;
    m=(l+r)/2;
    if(l>r)
        return -1;
    if(strcmp(label, st->tab[m].nome)==0)
        return m;
    if(strcmp(label, st->tab[m].nome)<0)
        return searchR(st, l, m-1, label);
    else
        return searchR(st, m+1, r, label);
}

int STsearch(ST st, char * label){
    return(searchR(st, 0, st->size, label));
}

char *STsearchByIndex(ST st, int index){
    return st->tab[index].nome;
}
