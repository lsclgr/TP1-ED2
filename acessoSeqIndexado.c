#include <stdio.h>

#define PAG_SIZE 4
#define TAB_SIZE 100

// definição de uma entrada da tabela de índice das páginas
typedef struct {
    int key;
} Index;

// definição de um item do arquivo de dados
typedef struct {
    char title[32];
    int key;
    float price;
} Item;

int search(Index tab[], int size, Item *item, FILE *fp)
{
    Item page[PAG_SIZE];
    int i, itemCount;
    long desloc;

    // procura pela página onde o item pode se encontrar
    i = 0;
    while (i < size && tab[i].key <= item->key) i++;

    // caso a key desejada seja menor que a 1a key, o item não existe no arquivo
    if (i == 0)
        return 0;
    else
    { // a ultima página pode não estar completa
        if (i < size)
        {
            itemCount = PAG_SIZE;
        }
        else
        {
            fseek(fp, 0, SEEK_END);
            itemCount = (ftell(fp) / sizeof(Item)) % PAG_SIZE;
        }

        // lê a página desejada do arquivo
        desloc = (i - 1) * PAG_SIZE * sizeof(Item);
        fseek(fp, desloc, SEEK_SET);
        fread(&page, sizeof(Item), itemCount, fp);

        // pesquisa sequencial na página lida
        for (i = 0; i < itemCount; i++)
        {
            if (page[i].key == item->key)
            {
                *item = page[i];
                return 1;
            }
        }

        return 0;
    }
}

int main()
{
    Index tabela[TAB_SIZE];
    FILE *fp;
    Item x;
    int pos, cont;

    // abre o arquivo de dados
    if ((fp = fopen("livros.bin", "rb")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        return 0;
    }

    // gera a tabela de índice das páginas
    cont = 0;
    pos = 0;
    while (fread(&x, sizeof(x), 1, fp) == 1)
    {
        cont++;
        if (cont % PAG_SIZE == 1)
        {
            tabela[pos].key = x.key;
            pos++;
        }
    }

    fflush(stdout);
    printf("Código do livro desejado: ");
    scanf("%d", &x.key);

    // ativa a função de pesquisa
    if (search(tabela, pos, &x, fp))
        printf("Livro %s (codigo %d) foi localizado", x.title, x.key);
    else
        printf("Livro de código %d nao foi localizado", x.key);

    fclose(fp);
    return 0;
}