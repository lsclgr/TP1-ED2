#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 
#include <time.h>
#define order 2
#define Disquete (order * 2)
#define FALSE 0
#define TRUE 1
#define len 5000

typedef int Key;

/* Estrutura Registro */
typedef struct Item {
    Key key;                                                    // Chave para efetuar a pesquisa 
    long int data1;                                             // Dado do tipo long
    char data2[len];                                                // Dado do tipo char referente a uma cadei de caracteres
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
        printf("%d\n",Ap->r[i - 1].key);
        exit(1);
        return true;
    }
    if (item->key < Ap->r[i - 1].key)                           // Chama a funcao recursivamente continuando
        Search(item, Ap->p[i - 1]);                             // a pesquisa através do filho a direita ou 
    else                                                        // a esquerda dependendo do valor da chave 
        Search(item, Ap->p[i]);
}
/* Funcao responsavel por inserir o registro na pagina */
void Insert_On_Page(Pointer Ap, Item item, Pointer ApDir) {
    bool position;                                              // Verifica se tem ou nao
    int k;
    k = Ap->n;
    
    while (k > 0) {                                             // Caso exista um ou mais registros na pagina referenciada, esse laço fica responsável por organizar os registros em ordem, de acordo com suas chaves
        if (item.key >= Ap->r[k - 1].key) {                     // Condicional que avalia se a chave do registro a ser inserido é maior que a analisada na pagina
            position = false;                                   
            break;
        }
        Ap->r[k] = Ap->r[k - 1];                                // Efetua o "empurramento" da chave ja existente na pagina para a proxima posicao caso necessário 
        Ap->p[k + 1] = Ap->p[k];
        k--;
        if (k < 1) position = false;
    }

    Ap->r[k] = item;                                            // Insercao do novo registro na pagina registro na pagina 
    Ap->p[k + 1] = ApDir;                                      
    Ap->n++;
}

void insert_Item(Item item, Pointer Ap, bool *grow_up, Item *item_Return, Pointer *ap_Return) {
    long i = 1, j;
    Pointer ap_time;

    if (Ap == NULL) {                                                   // Condicional que na primeira chamada verifica se a arvore e nula 
        *grow_up = TRUE;                                                // se sim, retorna para a funcao insert e inicializa uma arvore com a raiz
        *item_Return = item;                                            // sendo o resgistro a ser inserido
        *ap_Return = NULL;          
        return;
    }

    while (i < Ap->n && item.key > Ap->r[i - 1].key){                   // Laço responsável pela pesquisa dentro da pagina (semelhante à funcao "Search")
        i++;
    }   

    if (item.key == Ap->r[i - 1].key) {                                 // Condicional responsável por finalizar a funcao caso o registro a ser adicionado ja exista dentro da arvore
        printf(" Erro: Registro ja esta presente\n");
        *grow_up = FALSE;
        return;
    }

    if (item.key < Ap->r[i - 1].key) i--;                               // Condicional responsavel por decidir se iremos caminhar na subarvore à direita ou à esquerda

    insert_Item(item, Ap->p[i], grow_up, item_Return, ap_Return);       // Chamada recursiva da funcao baseada no resultado da condicional anterior 

    if (!*grow_up) {                                                    
        return;
    }

    if (Ap->n < Disquete){                                               // Condicional que avalia se a existe espaco livre na pagina
        Insert_On_Page(Ap, *item_Return, *ap_Return);
        *grow_up = FALSE;
        return;
    }
    
    ap_time = (Pointer)malloc(sizeof(Page));                            // Criacao de uma nova pagina caso nao exista espaco na pagina anterior para a insercao
    ap_time->n = 0;
    ap_time->p[0] = NULL;

    if (i < order + 1) {                                                // Condicional que define se o elemento a ser inserido será inserido na pagina ja existente ou na nova que foi criada
        Insert_On_Page(ap_time, Ap->r[Disquete - 1], Ap->p[Disquete]);
        Ap->n--;
        Insert_On_Page(Ap, *item_Return, *ap_Return);
    } else
        Insert_On_Page(ap_time, *item_Return, *ap_Return);

    for (j = order + 2; j <= Disquete; j++){                            // Laço responsável por efetuar a organização da pagina criada adicionando mais um item na pagina nova (nesse caso, pois pode variar dependendo da ordem da arvore) para não ferir a propriedade.
        Insert_On_Page(ap_time, Ap->r[j - 1], Ap->p[j]);
    }
    Ap->n = order;
    ap_time->p[0] = Ap->p[order + 1];
    *item_Return = Ap->r[order];
    *ap_Return = ap_time;
}
/* Funcao inicial de insercao de um registro na arvore e reponsavel por efetuar o crescimento da raiz da arvore */
void insert(Item item, Pointer *Ap) {
    bool grow_up;                                                       // Variavel que informa se apos a insercao houve ou nao um crescimento no nivel raiz da arvore, oua inda, em alguns casos, vai verificar se a insercao em um certo nivel da arvore modifica o nivel anterior (nodo pai)
    Item item_Return;                                                    // Registro que será retornado
    Page *ap_Return;                                                    // pagina filha à direita que será retornada
    Page *ap_time;

    insert_Item(item, *Ap, &grow_up, &item_Return, &ap_Return);
    if (grow_up) /* Arvore cresce na altura pela raiz */
    {
        ap_time = (Page *)malloc(sizeof(Page));
        ap_time->n = 1;
        ap_time->r[0] = item_Return;
        ap_time->p[1] = ap_Return;
        ap_time->p[0] = *Ap;
        *Ap = ap_time;
    }
}
/* Funcao que imprime a arvore criada. Ela faz a impressão da pagina pai, depois das páginas filhas à esquerda e depois às da direita */
void print_out(Pointer p, int nivel) {
    int i;
    
    if (p == NULL) {                                    // Caso base da recursão, quando o apontador chega em uma folha
        return;
    }

    printf("Altura %d : ", nivel);                       
    
    for (i = 0; i < p->n; i++){                         // Laço que imprime todos os registros da pagina
        printf("%d ", p->r[i].key);
    }
    
    printf("\n");
    nivel++;
    
    for (i = 0; i <= p->n; i++){                        // Laço que chama recursivamente a funcao mandando como parametro o endereço
        print_out(p->p[i], nivel);                      // de memoria da pagina filho mais a esquerda até o mais à direita e o nível posterior
    }                                                   
}

int menu(int operation){
    printf("Informe qual operacao deseja realizar:\n\n1 - Carregar arquivo\n2 - Pesquisar registro\n0 - Finalizar\n");
    scanf("%d",&operation);

    return operation;
}

int main(int argc, char *argv[]) {
    Item item_To_Search, item_To_Insert;
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
                        ret = fread(&item_To_Insert, sizeof(Item), 1, file);
                        if (ret == 0) break;
                        // inserir na arvore
                        //printf("Inseriu!!\n");
                        insert(item_To_Insert, &D);
                        // printf("%d\n", ret);
                    } while (ret != 0);
                    
                    print_out(D, nivel);
                    fclose(file);
                }
            }
            if(operation == 2){
                printf("Informe a chave do registro a ser pesquisado!\n");
                scanf("%d", &item_To_Search.key);
                printf("Opa\n");
                exit(1);
                search_Item = Search(&item_To_Search, D);
                if(!search_Item){
                    printf("Item nao esta presente na arvore\n");
                }
                else if(search_Item){
                    printf("Registro encontrado!\nDados:\n Chave: %d\nDado 1: %ld\nDado 2: \n", item_To_Search.key,item_To_Search.data1);
                }
            }
        }
    }
    return 0;
}