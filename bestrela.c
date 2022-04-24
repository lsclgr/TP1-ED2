#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define order 2
#define Disquete (order * 2)
#define FALSE 0
#define TRUE 1

typedef int Key;
typedef struct Item {
    Key key;
    long int data1;
    char *data2;
} Item;
typedef enum { Interna, Externa } TipoIntExt;
typedef struct Page *Pointer;
typedef struct Page {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            Key ri[order];
            Pointer pi[order + 1];
        } U0;
        struct {
            int ne;
            Item re[Disquete];
        } U1;
    } UU;
} Page;

void Search(Item *x, Pointer *Ap) {
    int i;
    Pointer Pag;
    Pag = *Ap;
    if ((*Ap)->Pt == Interna) {
        i = 1;
        while (i < Pag->UU.U0.ni && x->key > Pag->UU.U0.ri[i - 1]) i++;
        if (x->key < Pag->UU.U0.ri[i - 1])
            Search(x, &Pag->UU.U0.pi[i - 1]);
        else
            Search(x, &Pag->UU.U0.pi[i]);
        return;
    } else {
        i = 1;
        while (i < Pag->UU.U1.ne && x->key > Pag->UU.U1.re[i - 1].key) i++;
        if (x->key == Pag->UU.U1.re[i - 1].key)
            *x = Pag->UU.U1.re[i - 1];
        else
            printf("Item nao esta presente na arvore\n");
    }
}

void Insert_On_Page(Pointer Ap, Item item, Pointer ApDir) {
    short position;
    int k;
    k = Ap->UU.U1.ne;
    position = (k > 0);
    if (Ap->Pt == Interna) {
        k = Ap->UU.U0.ni;
        position = (k > 0);
        while (position) {
            if (item.key >= Ap->UU.U0.ri[k - 1].key) {
                position = FALSE;
                break;
            }
            Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
            Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
            k--;
            if (k < 1) position = FALSE;
        }
        Ap->UU.U0.ri[k] = item;
        Ap->UU.U0.pi[k + 1] = ApDir;
        Ap->UU.U0.ni++;
    } else {
        while (position) {
            if (item.key >= Ap->UU.U1.re[k - 1].key) {
                position = FALSE;
                break;
            }
            Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
            // Ap->p[k + 1] = Ap->p[k];
            k--;
            if (k < 1) position = FALSE;
        }
        Ap->UU.U1.re[k] = item;
        // Ap->p[k + 1] = ApDir;
        Ap->UU.U1.ne++;
    }
}

void insert_Item(Item item, Pointer Ap, bool *Cresceu, Item *itemReturn,
                 Pointer *ptReturn) {
    long i = 1;
    long j;
    Pointer ApTemp;
    if (Ap == NULL) {
        *Cresceu = TRUE;
        (*itemReturn) = item;
        // add tbm na pag externa
        ApTemp = (Page *)malloc(sizeof(Page));
        ApTemp->UU.U1.ne = 1;
        ApTemp->UU.U1.re[0] = item;
        ApTemp->Pt = Externa;
        (*ptReturn) = &ApTemp;
        return;
    }

    Pointer Pag;
    Pag = Ap;
    if (Ap->Pt == Interna) {
        while (i < Pag->UU.U0.ni && item.key > Pag->UU.U0.ri[i - 1]) i++;
    } else {
        while (i < Pag->UU.U1.ne && item.key > Pag->UU.U1.re[i - 1].key) i++;
        if (item.key == Pag->UU.U1.re[i - 1].key) {
            printf(" Erro: registro ja esta presente\n");
            *Cresceu = FALSE;
            return;
        }
    }

    if (item.key < Ap->r[i - 1].key) i--;
    insert_Item(item, Ap->p[i], Cresceu, itemReturn, ptReturn);
    if (!*Cresceu) return;

    if (Ap->n < Disquete) /* Pagina tem espaco */
    {
        Insert_On_Page(Ap, *itemReturn, *ptReturn);
        *Cresceu = FALSE;
        return;
    }
    /* Overflow: Pagina tem que ser dividida */
    ApTemp = (Pointer)malloc(sizeof(Page));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    if (i < order + 1) {
        Insert_On_Page(ApTemp, Ap->r[Disquete - 1], Ap->p[Disquete]);
        Ap->n--;
        Insert_On_Page(Ap, *itemReturn, *ptReturn);
    } else
        Insert_On_Page(ApTemp, *itemReturn, *ptReturn);

    for (j = order + 2; j <= Disquete; j++)
        Insert_On_Page(ApTemp, Ap->r[j - 1], Ap->p[j]);

    Ap->n = order;
    ApTemp->p[0] = Ap->p[order + 1];
    *itemReturn = Ap->r[order];
    *ptReturn = ApTemp;
}

void insert(Item item, Pointer *Ap) {
    bool Cresceu;
    Item itemReturn;
    Page *ptReturn, *ApTemp;
    insert_Item(item, *Ap, &Cresceu, &itemReturn, &ptReturn);
    if (Cresceu) /* Arvore cresce na altura pela raiz */
    {
        ApTemp = (Page *)malloc(sizeof(Page));
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = itemReturn;
        ApTemp->UU.U0.pi[1] = ptReturn;
        ApTemp->UU.U0.pi[0] = *Ap;
        ApTemp->Pt = Interna;
        *Ap = ApTemp;
    }
}
int main(int argc, char *argv[]) { return 0; }