#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define M 2
#define MM (M * 2)
#define FALSE 0
#define TRUE 1

typedef long Key;
typedef struct Item {
    Key key;
    long long int data1;
    char *data2;
    /*outros componentes*/
} Item;
typedef struct Page *Pointer;
typedef struct Page {
    short n;
    Item r[MM];
    Pointer p[MM + 1];
} Page;

bool Inicializa(Pointer *Dict) {
    *Dict = NULL;

    return true;
}

bool Search(Item *x, Pointer Ap) {
    long i = 1;
    if (Ap == NULL) {
        // printf("Item nao esta presente na arvore\n");
        return false;
    }
    while (i < Ap->n && x->key > Ap->r[i - 1].key) i++;
    if (x->key == Ap->r[i - 1].key) {
        *x = Ap->r[i - 1];
        return true;
    }
    if (x->key < Ap->r[i - 1].key)
        Search(x, Ap->p[i - 1]);
    else
        Search(x, Ap->p[i]);
}

void InsertOnPage(Pointer Ap, Item Reg, Pointer ApDir) {
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao) {
        if (Reg.key >= Ap->r[k - 1].key) {
            NaoAchouPosicao = FALSE;
            break;
        }
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
        k--;
        if (k < 1) NaoAchouPosicao = FALSE;
    }
    Ap->r[k] = Reg;
    Ap->p[k + 1] = ApDir;
    Ap->n++;
}

void Ins(Item Reg, Pointer Ap, short *Cresceu, Item *RegRetorno,
         Pointer *ApRetorno) {
    long i = 1;
    long j;
    Pointer ApTemp;
    if (Ap == NULL) {
        *Cresceu = TRUE;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    while (i < Ap->n && Reg.key > Ap->r[i - 1].key) i++;
    if (Reg.key == Ap->r[i - 1].key) {
        printf(" Erro: Registro ja esta presente\n");
        *Cresceu = FALSE;
        return;
    }
    if (Reg.key < Ap->r[i - 1].key) i--;
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
    if (!*Cresceu) return;
    if (Ap->n < MM) /* Pagina tem espaco */
    {
        InsertOnPage(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = FALSE;
        return;
    }
    /* Overflow: Pagina tem que ser dividida */
    ApTemp = (Pointer)malloc(sizeof(Page));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    if (i < M + 1) {
        InsertOnPage(ApTemp, Ap->r[MM - 1], Ap->p[MM]);
        Ap->n--;
        InsertOnPage(Ap, *RegRetorno, *ApRetorno);
    } else
        InsertOnPage(ApTemp, *RegRetorno, *ApRetorno);
    for (j = M + 2; j <= MM; j++) InsertOnPage(ApTemp, Ap->r[j - 1], Ap->p[j]);
    Ap->n = M;
    ApTemp->p[0] = Ap->p[M + 1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
}

void Insere(Item Reg, Pointer *Ap) {
    bool Cresceu;
    Item RegRetorno;
    Page *ApRetorno, *ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
    if (Cresceu) /* Arvore cresce na altura pela raiz */
    {
        ApTemp = (Page *)malloc(sizeof(Page));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
    }
}
void ImprimeI(Pointer p, int nivel) {
    long i;
    if (p == NULL) return;
    printf("Nivel %d : ", nivel);
    for (i = 0; i < p->n; i++) printf("%ld ", (long)p->r[i].key);
    putchar('\n');
    nivel++;
    for (i = 0; i <= p->n; i++) ImprimeI(p->p[i], nivel);
}

void Imprime(Pointer p) {
    int n = 0;
    ImprimeI(p, n);
}

int main(int argc, char *argv[]) {
    Item x;
    Page *D;
    FILE *arq;
    Inicializa(&D);
    arq = fopen("arquivo.bin", "rb+");
    if (!arq) {
        printf("Erro na abertura do arquivo. Fim de programa.");
        exit(1);
    }
    int ret;
    do {
        ret = fread(&x, sizeof(Item), 1, arq);
        if (ret == 0) break;
        // inserir na arvore
        printf("Inseriu!!\n");
        Insere(x, &D);
        Imprime(D);
        // printf("%d\n", ret);
    } while (ret != 0);
    fclose(arq);
    return 0;
}