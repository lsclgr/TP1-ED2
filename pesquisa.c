
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// estrutura registo
typedef struct registro
{
    int chave;
    long int dado1;
    char dado2[501];
} Registro;

typedef struct nodo
{
    Registro item;
    long int esq;
    long int dir;
} Nodo;


// estrutura para analise
typedef struct teste
{
    int numTrans;
    int numCompara;
    double tempArvore;
    double tempPesquisa;
    double tempTotal;
    clock_t tempInicioArvore;
    clock_t tempFimArvore;
    clock_t tempInicioPesquisa;
    clock_t tempFinalPesquisa;
} Testes;




// funcoes arvore binaria
void preencheNo(Nodo *n, Registro reg);
int criaArvoreBinaria(FILE *arqb, FILE *arvore, long tam, Testes *test);
int pesquisaBinariaArvore(FILE *arvore, Nodo *n, Registro *retorno, Testes *test);
void printBinArvore(FILE *arquivo1);

// funcoes de analise e de arquivo
void printReg(Registro reg);
int criaArquivo(int quant, int ordenacao);
void printArquivo(FILE *arquivo1);
void iniciaTestes(Testes *teste);
void contaTransferencia(Testes *teste);
void contaComparacao(Testes *teste);
void iniciaTempArvore(Testes *teste);
void finalTempArvore(Testes *teste);
void calculaTemparvore(Testes *teste);
void iniciaTempoPesquisa(Testes *teste);
void fimTempoPesquisa(Testes *teste);
void tempoPesquisaFinal(Testes *teste);
void tempoTotal(Testes *teste);
void printTestes(Testes *test);

//<método> <quantidade> <situação> <chave> [-P] 

int main(int argc, char *argv[])
{
    FILE *arquivo1;
    FILE *arquivo2;
    Registro reg;
    srand(time(NULL));

    if (argc < 5 || argc > 6) // verifica formato na linha de comado
    {
        printf("erro: o input correto é: pesquisa <metodo> <quantidade> <situacao> <chave>\n");
        exit(1);
    }

    // Variaveis da linha de execucao
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int auxPrint = 0;
    if (argc == 6)
    {
        if (!strcmp(argv[5], "[-P]"))
            auxPrint = 1;
        else
            printf("erro: o correto é '[-P]' \n.");
    }

    // A funcao criaArquivo retorna 0 caso o arquivo nao tenha sido criado
    if (!criaArquivo(quantidade, situacao))
    {
        printf("erro: O arquivo nao foi criado.\n");
        exit(1);
    }

    if ((arquivo1 = fopen("dados.bin", "rb")) == NULL)
    {
        printf("erro: nao foi possivel abrir o aquivo dados.bin.\n");
        exit(1);
    }
    reg.chave = chave;
    Testes test;
    iniciaTestes(&test);

    
    Nodo raiz; //raiz de um árvore binária
    long quantidadeLong = (long)quantidade;

    switch (metodo)
    {

        case 2: // Arvore binaria adequada a memoria externa
            if ((arquivo2 = fopen("temp.bin", "w+b")) == NULL)
            {
                printf("erro: O arquivo temp.bin nao pode ser aberto corretamente.\n");
                exit(1);
            }
            // construção da árvore binária
            iniciaTempArvore(&test);
            criaArvoreBinaria(arquivo1, arquivo2, quantidadeLong, &test);
            finalTempArvore(&test);

            // apagando o primeiro aquivo binario e renomeando o novo 
            fclose(arquivo1);
            fclose(arquivo2);
            if (remove("dados.bin") != 0)
                printf("erro ao deletar dados.bin antigo.\n");
            if (rename("temp.bin", "dados.bin") != 0)
                printf("erro ao renomear temp.bin.\n");
            if ((arquivo1 = fopen("dados.bin", "rb")) == NULL)
            {
                printf("erro: nao foi possivel abrir o dados.bin.\n");
                exit(1);
            }

            // Pesquisa
            iniciaTempoPesquisa(&test);
            if (pesquisaBinariaArvore(arquivo1, &raiz, &reg, &test))
            {
                fimTempoPesquisa(&test);
                if (auxPrint)
                    printBinArvore(arquivo1);
                printf("Registro com essa chave encontrado.\n");
                printReg(reg);
                printTestes(&test);
            }
            else
            {
                fimTempoPesquisa(&test);
                if (auxPrint)
                    printBinArvore(arquivo1);
                printf("Nao foi possivel achar um registro com essa chave.\n");
                printTestes(&test);
            }
            break;

        default:
            printf("erro: opcoes de pesquisa de 1 a 4.\n");
            break;
    }

    fclose(arquivo1);
    return 0;
}

// funcoes da arvore
// Preenche um nó com  -1 para os apontadores direita e esquerda e item é preenchido com o registro lido do arquivo.
void preencheNo(Nodo *n, Registro reg)
{
    n->dir = -1;
    n->esq = -1;
    n->item = reg;
}

int criaArvoreBinaria(FILE *arqb, FILE *arvore, long tam, Testes *test)
{
    Registro A;    // Registro lido a cada iteracao  
    Nodo auxiliar; // Nodo usado para caminha na arvore
    Nodo filho;

    
    int contador = 0;      // conta quantos registros foram inseridos e indicar a posicao no arquivo
    int posicaoFilho = 0;  // Armazena a posicao do filho esq/dir do nodo sendo analisado
    int continua = 1;          // continua enquanto for 1 define que deve continuar percorrendo na arvore
    int tamanhoArvore = 0; // quantidades de registro da arvore

    // Lê o registro do arquivo 
    contaTransferencia(test);
    while (fread(&A, sizeof(Registro), 1, arqb) == 1 && tamanhoArvore < tam)
    {
        contaTransferencia(test);
        continua = 1;
        preencheNo(&filho, A);

        // Realiza a gravação do Nodo criado no arquivo.
        contaTransferencia(test);
        fwrite(&filho, sizeof(Nodo), 1, arvore);

        contador++;

        // if para todos registros exceto primeiro.
        if (contador != 1)
        {
            rewind(arvore); // volta o ponteiro pro começo do arquivo
            posicaoFilho = 0;

            // Lendo a raiz da arvore e caminhando
            contaTransferencia(test);
            while (fread(&auxiliar, sizeof(Nodo), 1, arvore) == 1 && continua == 1)
            {
                contaTransferencia(test);
                // Compara se a o filho e menor ou maior
                contaComparacao(test);
                if (A.chave < auxiliar.item.chave)
                    //(-1) é sem filho.
                    if (auxiliar.esq == -1)
                    {
                        auxiliar.esq = contador - 1;
                        fseek(arvore, posicaoFilho * sizeof(Nodo), 0);
                        contaTransferencia(test);
                        fwrite(&auxiliar, sizeof(Nodo), 1, arvore);
                        continua = 0;
                    }
                    // se tem filho realiza o salto de acordo com o indice salvo em "esq".
                    else
                    {
                        fseek(arvore, (auxiliar.esq) * sizeof(Nodo), 0);
                        posicaoFilho = auxiliar.esq;
                    }
                else
                {
                    //(-1) é sem filho.
                    if (auxiliar.dir == -1)
                    {
                        auxiliar.dir = contador - 1;
                        fseek(arvore, posicaoFilho * sizeof(Nodo), 0);
                        contaTransferencia(test);
                        fwrite(&auxiliar, sizeof(Nodo), 1, arvore);
                        continua = 0;
                    }
                    // se tem filho realiza o salto de acordo com o indice salvo em "dir".
                    else
                    {
                        fseek(arvore, (auxiliar.dir) * sizeof(Nodo), 0);
                        posicaoFilho = auxiliar.dir;
                    }
                }
            }
        }
        fseek(arvore, 0, 2); // vai pro fim do arquivo para continuar na arvore

        tamanhoArvore++;
    }

    return 1;
}

// funcao que faz a pesquisa na arvore externa de forma recursiva 
int pesquisaBinariaArvore(FILE *arvore, Nodo *n, Registro *retorno, Testes *test)
{   
    fread(n, sizeof(Nodo), 1, arvore);
    contaTransferencia(test);

    // Item encontrado.
    if (n->item.chave == retorno->chave)
    {
        contaComparacao(test);
        *retorno = n->item;
        return 1;
    }

    else if (retorno->chave > n->item.chave && n->dir != -1)//direita
    {
        contaComparacao(test);
        fseek(arvore, (n->dir) * sizeof(Nodo), 0);
        return pesquisaBinariaArvore(arvore, n, retorno, test);
    }

    else if (retorno->chave < n->item.chave && n->esq != -1)//esquerda
    {
        contaComparacao(test);
        fseek(arvore, (n->esq) * sizeof(Nodo), 0);
        return pesquisaBinariaArvore(arvore, n, retorno, test);
    }

    // Item não encontrado.
    return 0;
}

// Imprime todas as chaves de pesquisa 
void printBinArvore(FILE *arquivo1)
{
    Nodo n;
    int i = 1;

    rewind(arquivo1);
    while (fread(&n, sizeof(Nodo), 1, arquivo1) == 1)
    {
        printf("Chave #%d: %d\n", i, n.item.chave);
        i++;
    }
    rewind(arquivo1);
}




// funcoes do arquivo e dos testes


//funcoes prints 
// A funcao printReg os dados do registro
void printReg(Registro reg)
{
    printf("REGISTRO\n");
    printf("Chave: %d\tDado 1: %ld\tDado 2: %s\n", reg.chave, reg.dado1, reg.dado2);
    
}

// Imprime todas as chaves presentes no arquivo
void printArquivo(FILE *arquivo1)
{
    Registro reg;
    int i = 1;

    rewind(arquivo1);
    while (fread(&reg, sizeof(Registro), 1, arquivo1) == 1)
    {
        printf("Chave %d: %d\n", i, reg.chave);
        i++;
    }
    rewind(arquivo1);
}

// Imprime a analise apos realizar uma pesquisa
void printTestes(Testes *test)
{
    printf("Tempo em ms.\n");
    printf("Tempo gasto no na construcao da tabela de indices ou da arvore : %lf\n", test->tempArvore);
    printf("Tempo pesquisa : %lf\n", test->tempPesquisa);
    printf("Tempo execucao : %lf\n", test->tempTotal);
    printf("Numero de comparacoes : %d\n", test->numCompara);
    printf("Numero de transferencias : %d\n", test->numTrans);
    
}


// A funcao constroiArquivo e responsavel pela construcao do arquivo binario "dados.bin" 
int criaArquivo(int quant, int ordenacao)
{
    FILE *binario;
    Registro reg;

    if ((binario = fopen("dados.bin", "w+b")) == NULL)
        return 0;

    // vetor utilizado para chaves aleatorias
    int vetor[quant];
    for (int i = 0; i < quant; i++){
        vetor[i] = i + 1;
    }
    int aux = quant;
    int aux2;

    for (int i = 0; i < quant; i++)
    {
        // criando um arquivo com chaves ordenadas crescentemente
        if (ordenacao == 1)
            reg.chave = i + 1;

        // criando um arquivo com chaves ordenadas decrescentemente
        else if (ordenacao == 2)
            reg.chave = quant - i;

        // criando um arquivo com chaves aleatorias 
        else
        {
            
            aux2 = rand() % aux;          // gera um numero aleatorio entre 0 e aux-1 ,aux tem quantidade (100, 1000 etc) e guarda em aux2
            reg.chave = vetor[aux2];      // chave recebe o valor da posicao aleatoria do vetor
            vetor[aux2] = vetor[aux - 1]; // vetor recebe o elemento da ultima posicao do vetor
            aux--;                        
        }

        // aleatorio de 1 a 10.000.000
        reg.dado1 = (rand() % 10000000) + 1;

        // tera o mesmo valor para todos os registros
        strcpy(reg.dado2, "mesmo dado para todos os registros.");
        fwrite(&reg, sizeof(Registro), 1, binario);
    }

    
    fclose(binario);
    return 1;
}

// Inicializa uma analise com 0 em todas posiçoes
void iniciaTestes(Testes *test)
{
    test->numTrans = 0;
    test->numCompara = 0;
    test->tempArvore = 0.0;
    test->tempPesquisa = 0.0;
    test->tempTotal = 0.0;
}

// soma 1 no numero de transferencias
void contaTransferencia(Testes *test)
{
    test->numTrans++;
}

// soma 1 no numero de comparacoes
void contaComparacao(Testes *test)
{
    test->numCompara++;
}


// Armazena o tempo do inicio da pesquisa 
void iniciaTempoPesquisa(Testes *test)
{
    test->tempInicioPesquisa = clock();
}

// Armazena o tempo do final da pesquisa e chama a funcao que calcula o tempo de pesquisa final e o tempo total
void fimTempoPesquisa(Testes *test)
{
    test->tempFinalPesquisa = clock();
    tempoPesquisaFinal(test);
    tempoTotal(test);
}


// Armazena o tempo do inicio da construcao da tabela de indices ou da arvore
void iniciaTempArvore(Testes *test)
{
    test->tempInicioArvore = clock();
}

// Armazena o tempo do final da construcao da tabela de indices ou da arvore e chama a funcao para calcular o tempo final 
void finalTempArvore(Testes *test)
{
    test->tempFimArvore = clock();
    calculaTemparvore(test);
}

// Calcula o tempo gasto na construcao da tabela de indices ou da arvore 
void calculaTemparvore(Testes *test)
{
    test->tempArvore = (test->tempFimArvore - test->tempInicioArvore) * 1000.0 / CLOCKS_PER_SEC;
}

// Calcula o tempo gasto na pesquisa propriamente 
void tempoPesquisaFinal(Testes *test)
{
    test->tempPesquisa = (test->tempFinalPesquisa - test->tempInicioPesquisa) * 1000.0 / CLOCKS_PER_SEC;
}

// Calcula o tempo total de execucao
void tempoTotal(Testes *test)
{
    test->tempTotal = test->tempArvore + test->tempPesquisa;
}

