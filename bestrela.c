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
// Tipo enumerado para identificar se uma pagina e interna (arvore B para armazenar os indices)
// ou uma pagina externa(acesso sequencial armazenando todos os registros)
typedef enum { Interna,
               Externa } TipoIntExt;
typedef struct Page *Pointer;
typedef struct Page {
    TipoIntExt Pt;
    union {
        struct {
            int ni;                    // numero de chaves dentro da pagina interna
            Key ri[DISQUETE];          // vetor de chaves na pagina interna
            Pointer pi[DISQUETE + 1];  // vetor de apontadores na pagina interna
        } U0;
        struct {
            int ne;             // numero de registros dentro da pagina externa
            Item re[DISQUETE];  // vetor de registros na pagina externa
        } U1;
    } UU;
} Page;

/* Função responsável por inicializar a árvore
onde e inicializada uma pagina externa com 0 registros armazenados*/
bool start_Tree(Pointer *Dict) {
    *Dict = (Page *)malloc(sizeof(Page));
    (*Dict)->Pt = Externa;
    (*Dict)->UU.U1.ne = 0;
    return true;
}
/* Função que efetua a pesquisa de um registro na árvore de maneira recursiva
funcao continua procurando ate que chegue em uma pagina externa onde os registros estao armazenados*/
bool Search(Item *x, Pointer *Ap) {
    int i;
    Pointer Pag;
    Pag = *Ap;
    if ((*Ap)->Pt == Interna) {
        i = 1;
        // compara com todos os itens dentro da pagina que ela esta
        // e e chamada novamente para o devido apontador caso seja maior ou menor
        while (i < Pag->UU.U0.ni && x->key > Pag->UU.U0.ri[i - 1]) i++;
        if (x->key < Pag->UU.U0.ri[i - 1])
            Search(x, &Pag->UU.U0.pi[i - 1]);
        else
            Search(x, &Pag->UU.U0.pi[i]);
    } else {
        // caso chegue na pagina externa, procura se o item desejado esta naquela pagina, se nao estiver,
        // significa que ele nao existe e retorna falso
        i = 1;
        while (i < Pag->UU.U1.ne && x->key > Pag->UU.U1.re[i - 1].key) i++;
        if (x->key == Pag->UU.U1.re[i - 1].key) {
            *x = Pag->UU.U1.re[i - 1];
        } else {
            return false;
        }
    }
    return true;
}

/*Funcao para inserir itens em uma pagina interna*/
void Insert_On_Intern_Page(Pointer Ap, Key key, Pointer ApDir) {
    if (Ap->Pt == Externa) return;
    int k;
    k = Ap->UU.U0.ni;
    int i = 1;
    // Caso exista um ou mais registros na pagina referenciada, esse laço fica responsável
    // por organizar os registros em ordem, de acordo com suas chaves
    while (i < Ap->UU.U0.ni && key > Ap->UU.U0.ri[i - 1]) i++;
    if (Ap->UU.U0.ni != 0 && key == Ap->UU.U0.ri[i - 1]) return;

    if (Ap->UU.U0.ni == 0 || key < Ap->UU.U0.ri[i - 1]) i--;
    // Efetua o "empurramento" da chave ja existente na pagina para a proxima posicao caso necessário
    for (int n = k; n > i; n--) {
        Ap->UU.U0.ri[n] = Ap->UU.U0.ri[n - 1];
        Ap->UU.U0.pi[n + 1] = Ap->UU.U0.pi[n];
    }
    // Insercao da chave na pagina interna
    Ap->UU.U0.ri[i] = key;
    Ap->UU.U0.pi[i + 1] = ApDir;
    Ap->UU.U0.ni++;
}

/*Funcao para inserir itens em uma pagina interna*/
void Insert_On_Extern_Page(Pointer Ap, Item item, bool *Cresceu, Item *itemReturn, Pointer *ptReturn) {
    if (Ap->Pt == Interna) return;
    int k = Ap->UU.U1.ne;
    int i = 1;

    // Caso exista um ou mais registros na pagina referenciada, esse laço fica responsável
    // por organizar os registros em ordem, de acordo com suas chaves
    while (i < Ap->UU.U1.ne && item.key > Ap->UU.U1.re[i - 1].key) i++;
    if (item.key == Ap->UU.U1.re[i - 1].key && k != 0) return;

    // Caso tenha espaco na pagina
    if (Ap->UU.U1.ne < DISQUETE) {
        *Cresceu = FALSE;
        // Efetua o "empurramento" da chave ja existente na pagina para a proxima posicao caso necessário
        if (k == 0 || item.key < Ap->UU.U1.re[i - 1].key) i--;
        for (int n = k; n > i; n--) {
            Ap->UU.U1.re[n] = Ap->UU.U1.re[n - 1];
        }
        // Insercao do registro na pagina externa
        Ap->UU.U1.re[i] = item;
        Ap->UU.U1.ne++;
    } else {  // Caso nao tenha espaco na pagina
        // cria uma nova pagina do tipo externa e inicializa com 0 elementos
        Pointer ApTemp = (Page *)malloc(sizeof(Page));
        ApTemp->Pt = Externa;
        ApTemp->UU.U1.ne = 0;
        i--;
        // verifica se a prosicao do i e menor que o meio da pagina para dividi-la pela metade
        if (i < ORDER) {
            // aqui sao inseridos os elementos da metade para o fim na nova pagina chamando recursivamente
            // a funcao de inserir na pagina externa passando o endereco da nova pagina criada
            Insert_On_Extern_Page(ApTemp, Ap->UU.U1.re[DISQUETE - 1], Cresceu, itemReturn, ptReturn);
            // assim que o elemento e inserido na nova pagina, o numero de elementos da antiga pagina diminui
            Ap->UU.U1.ne--;
            // como agora tem espaco na antiga pagina, o item que queriamos inserir agora e inserido na pagina
            Insert_On_Extern_Page(Ap, item, Cresceu, itemReturn, ptReturn);
        } else {
            // se i for maior que o meio da pagina o elemento de i apenas e inserido na nova
            Insert_On_Extern_Page(ApTemp, item, Cresceu, itemReturn, ptReturn);
        }
        // aqui sao passados todos os elementos finais para a nova pagina, inclusive o elemento do meio que sera
        // enviado para a pagina interna como indice alem de ser inserido na nova pagina externa
        for (int j = ORDER + 1; j <= DISQUETE; j++) {
            Insert_On_Extern_Page(ApTemp, Ap->UU.U1.re[j - 1], Cresceu, itemReturn, ptReturn);
        }
        // pagina atual atualizada com o numero de elementos igual a metade arredondando para baixo
        // e retornando o item do meio para inseri-lo na pagina interna, dividindo em duas paginas, uma com
        // elementos maiores que ele na direita e outra na esquerda com menores
        Ap->UU.U1.ne = ORDER;
        itemReturn->key = Ap->UU.U1.re[ORDER].key;
        *ptReturn = ApTemp;
        *Cresceu = TRUE;
    }
}

void insert_Item(Item item, Pointer Ap, bool *Cresceu, Item *itemReturn, Pointer *ptReturn) {
    long i = 1;
    long j;
    Pointer ApTemp;

    if (Ap->Pt == Interna) {
        // Laço responsável pela pesquisa dentro da pagina (semelhante à funcao "Search")
        while (i < Ap->UU.U0.ni && item.key > Ap->UU.U0.ri[i - 1]) i++;
        if (item.key < Ap->UU.U0.ri[i - 1]) i--;
        insert_Item(item, Ap->UU.U0.pi[i], Cresceu, itemReturn, ptReturn);
        if (!*Cresceu) return;

        // Caso ainda tenha espaco na pagina, o item e inserido normalmente
        if (Ap->UU.U0.ni < DISQUETE) {
            Insert_On_Intern_Page(Ap, itemReturn->key, *ptReturn);
            *Cresceu = FALSE;
            return;
        }
        // Caso a pagina esteja cheia
        // e criada uma nova pagina interna temporaria apontando para null
        ApTemp = (Pointer)malloc(sizeof(Page));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;

        // assim como na funcao de inserir na arvore externa, os elementos do meio para
        // frente da pagina sao inseridos na pagina auxiliar
        if (i <= ORDER) {
            // aqui sao inseridos os elementos da metade para o fim na nova pagina chamando recursivamente
            // a funcao de inserir na pagina interna passando o endereco da nova pagina criada
            Insert_On_Intern_Page(ApTemp, Ap->UU.U0.ri[DISQUETE - 1], Ap->UU.U0.pi[DISQUETE]);
            // assim que o elemento e inserido na nova pagina, o numero de elementos da antiga pagina diminui
            Ap->UU.U0.ni--;
            // como agora tem espaco na antiga pagina, o item que queriamos inserir agora e inserido na pagina
            Insert_On_Intern_Page(Ap, itemReturn->key, *ptReturn);
        } else {
            // se i for maior que o meio da pagina o elemento de i apenas e inserido na nova
            Insert_On_Intern_Page(ApTemp, itemReturn->key, *ptReturn);
        }
        // aqui sao passados todos os elementos para a nova pagina, menos o elemento do meio que sera
        // enviado para a nova pagina interna pai como indice
        for (j = ORDER + 2; j <= DISQUETE; j++) {
            Insert_On_Intern_Page(ApTemp, Ap->UU.U0.ri[j - 1], Ap->UU.U0.pi[j]);
        }
        // pagina atual atualizada com o numero de elementos igual a metade arredondando para baixo
        // e retornando o item do meio para inseri-lo na pagina interna pai, dividindo em duas paginas, uma com
        // elementos maiores que ele na direita e outra na esquerda com menores
        Ap->UU.U0.ni = ORDER;
        ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[ORDER + 1];
        itemReturn->key = Ap->UU.U0.ri[ORDER];
        *ptReturn = ApTemp;
        *Cresceu = TRUE;
    } else {
        // caso a pagina seja externa, a funcao de inserir na pagina externa e chamada
        Insert_On_Extern_Page(Ap, item, Cresceu, itemReturn, ptReturn);
    }
}

void insert(Item item, Pointer *Ap) {
    bool Cresceu;
    Item itemReturn;
    Page *ptReturn, *ApTemp;

    insert_Item(item, *Ap, &Cresceu, &itemReturn, &ptReturn);
    if (Cresceu) /* Arvore cresce na altura pela raiz cria uma nova pagina interna com
    a chave retornada e dois apontadores: a pagina externa que foi dividida e a nova pagina externa que foi criada*/
    {
        ApTemp = (Page *)malloc(sizeof(Page));
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = itemReturn.key;
        ApTemp->UU.U0.pi[1] = ptReturn;
        ApTemp->UU.U0.pi[0] = *Ap;
        ApTemp->Pt = Interna;
        *Ap = ApTemp;
    }
}
void print_out(Pointer p, int nivel) {
    int i;

    if (p->Pt == Externa) {  // Caso base da recursão, quando o apontador chega
        // em uma folha
        printf("Externa: ", (nivel + 1));
        for (int i = 0; i < p->UU.U1.ne; i++) {
            printf("%d ", p->UU.U1.re[i].key);
        }
        printf("\n");
        return;
    }
    // if (nivel == 1) {
    //     printf("Altura %d : ", nivel);
    // } else
    printf("Altura %d : ", nivel);

    for (i = 0; i < p->UU.U0.ni; i++) {  // Laço que imprime todos os registros da pagina
        printf("%d ", p->UU.U0.ri[i]);
    }

    printf("\n");
    nivel++;

    for (i = 0; i <= p->UU.U0.ni;
         i++) {  // Laço que chama recursivamente a funcao
                 // mandando como parametro o endereço
        print_out(p->UU.U0.pi[i],
                  nivel);  // de memoria da pagina filho mais a esquerda até o
                           // mais à direita e o nível posterior
    }
}

int menu(int operation) {
    printf(
        "\nInforme qual operacao deseja realizar:\n\n1 - Carregar arquivo\n2 - "
        "Pesquisar registro\n0 - Finalizar\n");
    scanf("%d", &operation);

    return operation;
}

int main() {
    int cont = 0;
    Item item_To_Search, item_To_Insert;
    Page *D;
    FILE *file;
    bool startup = false, search_Item;
    int operation = -1, ret, nivel = 0;

    while (operation != 0) {
        item_To_Search.data2[0] = '0';
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
                        cont++;
                        ret = fread(&item_To_Insert, sizeof(Item), 1, file);
                        if (ret == 0) break;
                        // inserir na arvore
                        insert(item_To_Insert, &D);

                        // printf("Inseriu!\nDados:\nChave: %d\nDado 1: %ld\nDado 2: %s\n", item_To_Insert.key, item_To_Insert.data1, item_To_Insert.data2);
                        //  printf("%d\n", ret);
                    } while (ret != 0);
                    print_out(D, nivel);
                    fclose(file);
                }
            }
            if (operation == 2) {
                printf("Informe a chave do registro a ser pesquisado!\n");
                scanf("%d", &item_To_Search.key);
                search_Item = Search(&item_To_Search, &D);
                if (!search_Item || item_To_Search.data2[0] == '0') {
                    printf("Item nao esta presente na arvore\n");
                } else if (search_Item) {
                    printf("Registro encontrado!\nDados:\n Chave: %d\nDado 1: %ld\nDado 2: %s\n", item_To_Search.key, item_To_Search.data1, item_To_Search.data2);
                }
            }
        }
    }
    return 0;
}