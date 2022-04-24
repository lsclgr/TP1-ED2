#include <stdio.h>
#include <stdlib.h>
#define MM 1
#define MM2 2

typedef int Key;
typedef struct Item {
    Key key;
    long int data1;
    char *data2;
} Item;
typedef enum { Interna, Externa } TipoIntExt;
typedef struct TipoPagina *TipoApontador;
typedef struct TipoPagina {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            Key ri[MM];
            TipoApontador pi[MM + 1];
        } U0;
        struct {
            int ne;
            Item re[MM2];
        } U1;
    } UU;
} TipoPagina;

void Pesquisa(Item *x, TipoApontador *Ap) {
    int i;
    TipoApontador Pag;
    Pag = *Ap;
    if ((*Ap)->Pt == Interna) {
        i = 1;
        while (i < Pag->UU.U0.ni && x->key > Pag->UU.U0.ri[i - 1]) i++;
        if (x->key < Pag->UU.U0.ri[i - 1])
            Pesquisa(x, &Pag->UU.U0.pi[i - 1]);
        else
            Pesquisa(x, &Pag->UU.U0.pi[i]);
        return;
    }
    i = 1;
    while (i < Pag->UU.U1.ne && x->key > Pag->UU.U1.re[i - 1].key) i++;
    if (x->key == Pag->UU.U1.re[i - 1].key)
        *x = Pag->UU.U1.re[i - 1];
    else
        printf("Item nao esta presente na arvore\n");
}

int main(int argc, char *argv[]) { return 0; }