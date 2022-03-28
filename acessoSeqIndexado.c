#include <stdio.h>

#define ITENSPAGINA 4
#define MAXTABELA 100
// definição de uma entrada da tabela de índice das páginas
typedef struct {
    int key;
} tipoindice;
// definição de um item do arquivo de dados
typedef struct {
    char title[31];
    int key;
    float price;
} itenType;

int pesquisa(tipoindice tab[], int tam, itenType* item, FILE* arq) {
    itenType pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;
    // procura pela página onde o item pode se encontrar
    i = 0;
    while (i < tam && tab[i].key <= item->key) i++;
    // caso a key desejada seja menor que a 1a key, o item
    // não existe no arquivo
    if (i == 0)
        return 0;
    else {  // a ultima página pode não estar completa
        if (i < tam)
            quantitens = ITENSPAGINA;
        else {
            fseek(arq, 0, SEEK_END);
            quantitens = (ftell(arq) / sizeof(itenType)) % ITENSPAGINA;
        }
        // lê a página desejada do arquivo
        desloc = (i - 1) * ITENSPAGINA * sizeof(itenType);
        fseek(arq, desloc, SEEK_SET);
        fread(&pagina, sizeof(itenType), quantitens, arq);
        // pesquisa sequencial na página lida
        for (i = 0; i < quantitens; i++)
            if (pagina[i].key == item->key) {
                *item = pagina[i];
                return 1;
            }
        return 0;
    }
}

int main() {
    tipoindice tabela[MAXTABELA];
    FILE* arq;
    itenType x;
    int pos, cont;
    // abre o arquivo de dados
    if ((arq = fopen("livros.bin", "rb")) == NULL) {
        printf("Erro na abertura do arquivo\n");
        return 0;
    }
    // gera a tabela de índice das páginas
    cont = 0;
    pos = 0;
    while (fread(&x, sizeof(x), 1, arq) == 1) {
        cont++;
        if (cont % ITENSPAGINA == 1) {
            tabela[pos].key = x.key;
            pos++;
        }
    }
    fflush(stdout);
    printf("Código do livro desejado: ");
    scanf("%d", &x.key);
    // ativa a função de pesquisa
    if (pesquisa(tabela, pos, &x, arq))
        printf("Livro %s (codigo %d) foi localizado", x.title, x.key);
    else
        printf("Livro de código %d nao foi localizado", x.key);
    fclose(arq);
    return 0;
}