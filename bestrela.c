#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define ORDER 2
#define DISQUETE (ORDER * 2)
#define FALSE 0
#define TRUE 1
#define LEN 2000

typedef int Key;
typedef struct Item {
    Key key;
    long int data1;
    char data2[LEN];
} Item;
typedef enum { Interna, Externa } TipoIntExt;
typedef struct Page *Pointer;
typedef struct Page {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            Key ri[ORDER];
            Pointer pi[ORDER + 1];
        } U0;
        struct {
            int ne;
            Item re[DISQUETE];
        } U1;
    } UU;
} Page;

/* Função responsável por inicializar a árvore */
bool start_Tree(Pointer *Dict) {
    Dict = (Page *)malloc(sizeof(Page));
    (*Dict)->Pt = Externa;
    (*Dict)->UU.U1.ne = 0;
    return true;
}

bool Search(Item *x, Pointer *Ap) {
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
        return true;
    } else {
        i = 1;
        while (i < Pag->UU.U1.ne && x->key > Pag->UU.U1.re[i - 1].key) i++;
        if (x->key == Pag->UU.U1.re[i - 1].key)
            *x = Pag->UU.U1.re[i - 1];
        else {
            return false;
        }
    }
    return false;
}

void Insert_On_Intern_Page(Pointer Ap, Key key, Pointer ApDir) {
    if (Ap->Pt == Externa) return;
    short position;
    int k;
    k = Ap->UU.U0.ni;
    position = (k > 0);
    while (position) {
        if (key >= Ap->UU.U0.ri[k - 1]) {
            position = FALSE;
            break;
        }
        Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
        Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
        k--;
        if (k < 1) position = FALSE;
    }
    Ap->UU.U0.ri[k] = key;
    Ap->UU.U0.pi[k + 1] = ApDir;
    Ap->UU.U0.ni++;
}
void Insert_On_Extern_Page(Pointer Ap, Item item, bool *Cresceu,
                           Item *itemReturn, Pointer *ptReturn) {
    if (Ap->Pt == Interna) return;
    short position;
    int k;
    k = Ap->UU.U1.ne;
    position = (k > 0);

    if (k < DISQUETE) {
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
    } else {
        if (i < ORDER + 1) {
            Insert_On_Extern_Page(ApTemp, Ap->UU.U1.re[DISQUETE - 1]);
            Ap->UU.U1.ne--;
            Insert_On_Extern_Page(Ap, *itemReturn);
        } else
            Insert_On_Extern_Page(ApTemp, *itemReturn);

        for (j = ORDER + 1; j <= DISQUETE; j++)
            Insert_On_Extern_Page(ApTemp, Ap->UU.U1.re[j - 1]);

        // Ap->UU.U1.ne = ORDER;
        *itemReturn = Ap->UU.U1.re[i];
        *ptReturn = ApTemp;
        *Cresceu = TRUE;
        return;
    }
}

void insert_Item(Item item, Pointer Ap, bool *Cresceu, Item *itemReturn,
                 Pointer *ptReturn) {
    long i = 1;
    long j;
    Pointer ApTemp;
    // if (Ap == NULL) {
    //     *Cresceu = TRUE;
    //     (*itemReturn) = item;
    //     // add tbm na pag externa
    //     ApTemp = (Page *)malloc(sizeof(Page));
    //     ApTemp->UU.U1.ne = 1;
    //     ApTemp->UU.U1.re[0] = item;
    //     ApTemp->Pt = Externa;
    //     (*ptReturn) = ApTemp;
    //     return;
    // }
    (*ptReturn) = ApTemp;
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

    if (Ap->Pt == Interna && item.key < Ap->UU.U0.ri[i - 1]) {
        insert_Item(item, Ap->UU.U0.pi[--i], Cresceu, itemReturn, ptReturn);
    }
    if (Ap->Pt == Externa && item.key < Ap->UU.U1.re[i - 1].key) {
        insert_Item(item, Ap->UU.U0.pi[--i], Cresceu, itemReturn, ptReturn);
    }
    if (!*Cresceu) return;

    if (Ap->UU.U1.ne < DISQUETE) { /* Pagina tem espaco */
        Insert_On_Extern_Page(Ap, *itemReturn);
        *Cresceu = FALSE;
        return;
    }
    /* Overflow: Pagina tem que ser dividida */
    if (Ap->Pt == Interna) {
        ApTemp = (Page *)malloc(sizeof(Page));
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;

        if (i < ORDER + 1) {
            Insert_On_Intern_Page(ApTemp, Ap->UU.U0.ri[DISQUETE - 1],
                                  Ap->UU.U0.pi[DISQUETE]);
            Ap->UU.U0.ni--;
            Insert_On_Intern_Page(Ap, itemReturn->key, *ptReturn);
        } else
            Insert_On_Intern_Page(ApTemp, itemReturn->key, *ptReturn);

        for (j = ORDER + 2; j <= DISQUETE; j++)
            Insert_On_Intern_Page(ApTemp, Ap->UU.U0.ri[j - 1], Ap->UU.U0.pi[j]);

        Ap->UU.U0.ni = ORDER;
        ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[ORDER + 1];
        itemReturn->key = Ap->UU.U0.ri[ORDER];
        *ptReturn = ApTemp;
        *Cresceu = TRUE;
        return;
    } else {
        ApTemp = (Page *)malloc(sizeof(Page));
        ApTemp->UU.U1.ne = 0;
        if (i < ORDER + 1) {
            Insert_On_Extern_Page(ApTemp, Ap->UU.U1.re[DISQUETE - 1]);
            Ap->UU.U1.ne--;
            Insert_On_Extern_Page(Ap, *itemReturn);
        } else
            Insert_On_Extern_Page(ApTemp, *itemReturn);

        for (j = ORDER + 1; j <= DISQUETE; j++)
            Insert_On_Extern_Page(ApTemp, Ap->UU.U1.re[j - 1]);

        Ap->UU.U1.ne = ORDER;
        *itemReturn = Ap->UU.U1.re[ORDER];
        *ptReturn = ApTemp;
        *Cresceu = TRUE;
        return;
    }
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
        ApTemp->UU.U0.ri[0] = itemReturn.key;
        ApTemp->UU.U0.pi[0] = *Ap;
        ApTemp->UU.U0.pi[1] = ptReturn;
        ApTemp->Pt = Interna;
        *Ap = ApTemp;
    }
}
void print_out(Pointer p, int nivel) {
    if (p->Pt == Externa) {  // Caso base da recursão, quando o apontador chega
        // em uma folha
        printf("Externa: ");
        for (int i = 0; i < p->UU.U1.ne; i++) {
            printf("%d ", p->UU.U1.re[i].key);
        }
        return;
    }

    printf("Altura %d : ", nivel);

    for (int i = 0; i < p->UU.U0.ni;
         i++) {  // Laço que imprime todos os registros da pagina
        printf("%d ", p->UU.U0.ri[i]);
    }

    printf("\n");
    nivel++;

    for (int i = 0; i <= p->UU.U0.ni;
         i++) {  // Laço que chama recursivamente a funcao
                 // mandando como parametro o endereço
        print_out(p->UU.U0.pi[i],
                  nivel);  // de memoria da pagina filho mais a esquerda até o
                           // mais à direita e o nível posterior
    }
}

int menu(int operation) {
    printf(
        "Informe qual operacao deseja realizar:\n\n1 - Carregar arquivo\n2 - "
        "Pesquisar registro\n0 - Finalizar\n");
    scanf("%d", &operation);

    return operation;
}

int main() {
    Item item_To_Search, item_To_Insert;
    Page *D;
    FILE *file;
    bool startup = false, search_Item;
    int operation = -1, ret, nivel = 0;

    while (operation != 0) {
        operation = menu(operation);
        if (operation == 2 && startup == false) {
            system("clear");
            printf("Registro não encontrado!\n\n");
        } else {
            if (operation == 1) {
                startup = start_Tree(&D);
                file = fopen("arquivo.bin", "rb+");
                if (!file) {
                    printf("Erro na abertura do arquivo. Fim de programa.");
                    exit(1);
                }
                if (!startup) {
                    printf("Não foi possível inicializar a árvore !\n");
                    exit(1);
                } else {
                    do {
                        ret = fread(&item_To_Insert, sizeof(Item), 1, file);
                        if (ret == 0) break;
                        // inserir na arvore
                        // printf("Inseriu!!\n");
                        insert(item_To_Insert, &D);
                        print_out(D, nivel);
                        // printf("%d\n", ret);
                    } while (ret != 0);

                    print_out(D, nivel);
                    fclose(file);
                }
            }
            if (operation == 2) {
                printf("Informe a chave do registro a ser pesquisado!\n");
                scanf("%d", &item_To_Search.key);
                search_Item = Search(&item_To_Search, &D);
                if (!search_Item) {
                    printf("Item nao esta presente na arvore\n");
                } else if (search_Item) {
                    printf(
                        "Registro encontrado!\nDados:\n Chave: %d\nDado 1: "
                        "%ld\nDado 2: %s\n",
                        item_To_Search.key, item_To_Search.data1,
                        item_To_Search.data2);
                }
            }
        }
    }
    return 0;
}