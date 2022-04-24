#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define order 2
#define Disquete (order * 2)
#define FALSE 0
#define TRUE 1

typedef int Key;

/* Estrutura Registro */
typedef struct Item {
    Key key;                                                    // Chave para efetuar a pesquisa 
    long int data1;                                             // Dado do tipo long
    char *data2;                                                // Dado do tipo char referente a uma cadei de caracteres
} Item;

typedef struct Page *Pointer;

/* Estrutura página */
typedef struct Page {
    int n;                                                      // Número de registro contidos na página
    Item r[Disquete];                                           // Array de registros 
    Pointer p[Disquete + 1];                                    // Array de apontadores que apontam para os endereços de memória das páginas filhas (quando existem)
} Page;

/* Função responsável por inicializar a árvore */
bool start_Tree(Pointer *Dict) {
    *Dict = NULL;
    return true;
}

/* Função que efetua a pesquisa de um registro na árvore de maneira recursiva */
bool Search(Item *item, Pointer Ap) {
    int i = 1;
    if (Ap == NULL) {                                           // Indica que ou a árvore não existe (se cair aqui na primeira chamada da função) ou o registro não está na árvore (caso chegue a uma pag folha)
        return false;
    }
    while (i < Ap->n && item->key > Ap->r[i - 1].key){          // Laço responsável pela pesquisa dentro da página
        i++;
    }
    if (item->key == Ap->r[i - 1].key) {                        // Condicional responsável por analisar se a cave do registro analisado é igual a do solicitado
        *item = Ap->r[i - 1];
        return true;
    }
    if (item->key < Ap->r[i - 1].key)                           // Chama a funcao recursivamente continuando
        Search(item, Ap->p[i - 1]);                             // a pesquisa através do filho a direita ou 
    else                                                        // a esquerda dependendo do valor da chave 
        Search(item, Ap->p[i]);
}

void Insert_On_Page(Pointer Ap, Item Reg, Pointer ApDir) {
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

void insert_Item(Item Reg, Pointer Ap, bool *Cresceu, Item *RegRetorno,
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
    insert_Item(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
    if (!*Cresceu) return;
    if (Ap->n < Disquete) /* Pagina tem espaco */
    {
        Insert_On_Page(Ap, *RegRetorno, *ApRetorno);
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
        Insert_On_Page(Ap, *RegRetorno, *ApRetorno);
    } else
        Insert_On_Page(ApTemp, *RegRetorno, *ApRetorno);
    for (j = order + 2; j <= Disquete; j++)
        Insert_On_Page(ApTemp, Ap->r[j - 1], Ap->p[j]);
    Ap->n = order;
    ApTemp->p[0] = Ap->p[order + 1];
    *RegRetorno = Ap->r[order];
    *ApRetorno = ApTemp;
}

void insert(Item item, Pointer *Ap) {
    bool Cresceu;
    Item RegRetorno;
    Page *ApRetorno, *ApTemp;
    insert_Item(item, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
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
/* Funcao que imprime a arvore criada. Ela faz a impressão da pagina pai, depois das páginas filhas à esquerda e depois às da direita */
void print_out(Pointer p, int nivel) {
    int i;
    
    if (p == NULL) {                                    // Caso base da recursão, quando o apontador chega em uma folha
        return;
    }

    printf("Nivel %d : ", nivel);                       
    
    for (i = 0; i < p->n; i++){                         // Laço que imprime todos os registros da pagina
        printf("%d ", p->r[i].key);
    }
    
    printf("\n");
    nivel++;
    
    for (i = 0; i <= p->n; i++){                        // Laço que chama recursivamente a funcao mandando como parametro o endereço
        print_out(p->p[i], nivel);                      // de memoria da pagina filho mais a esquerda até o mais à direita
    }                                                   
}

int menu(int operation){
    printf("Informe qual operacao deseja realizar:\n\n1 - Carregar arquivo\n2 - Pesquisar registro\n0 - Finalizar\n");
    scanf("%d",&operation);

    return operation;
}

int main(int argc, char *argv[]) {
    Item item_To_Search;
    Page *D;
    FILE *file;
    bool startup = false, search_Item;
    int operation = -1, ret, nivel = 0;
    
    
    while(operation != 0){
        operation = menu(operation);
        if(operation == 2 && startup == false){
            system("clear");
            printf("Registro não encontrado!\n\n");
        }
        else{
            if(operation == 1){
                startup = start_Tree(&D);
                file = fopen("arquivo.bin", "rb+");
                if (!file){
                    printf("Erro na abertura do arquivo. Fim de programa.");
                    exit(1);
                }
                if(!startup){
                    printf("Não foi possível inicializar a árvore !\n");
                    exit(1);
                }
                else{
                    do {
                        ret = fread(&item_To_Search, sizeof(Item), 1, file);
                        if (ret == 0) break;
                        // inserir na arvore
                        //printf("Inseriu!!\n");
                        insert(item_To_Search, &D);
                        // printf("%d\n", ret);
                    } while (ret != 0);
                    print_out(D, nivel);
                    fclose(file);
                }
            }
            if(operation == 2){
                printf("Informe a chave do registro a ser pesquisado!\n");
                scanf("%d", item_To_Search.key);
                search_Item = Search(&item_To_Search, D);
                if(!search_Item){
                    printf("Item nao esta presente na arvore\n");
                }
                else if(search_Item){

                }
            }
        }
    }
    return 0;
}