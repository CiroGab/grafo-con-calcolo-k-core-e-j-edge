#ifndef ESAMEAPA_ST_H
#define ESAMEAPA_ST_H

#define MAXC 10

typedef struct symbtab *ST;
typedef struct{
    char nome[MAXC];
}Item;

ST STinit(int maxN);
void STinsert(ST st, Item val);
int STsearch(ST st, char *label);
char *STsearchByIndex(ST st, int index);
void STfree(ST st);

#endif
