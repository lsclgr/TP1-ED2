#include <stdio.h>
#include <stdlib.h> /* Para a função exit() */
#include <time.h>

typedef struct {
    int chave;
    long long int dado1;
    char *dado2;
} info;
void gen_random(char *s, const int len) {
    srand(time(NULL));
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len - 1; i++) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len - 1] = '\0';
}

int main() {
    FILE *arq;

    srand(time(NULL));
    arq = fopen("arquivo.dat", "wb");
    if (!arq) {
        printf("Erro na abertura do arquivo. Fim de programa.");
        exit(1);
    }
    int len = 5000, tam = 100, ret;
    info x;

    for (int i = 0; i < tam; i++) {
        x.chave = i;
        x.dado1 = rand() % 2147483647;
        usleep(1000000);
        printf("%lld\n", x.dado1);
        x.dado2 = (char *)malloc(len * sizeof(char));
        gen_random(x.dado2, len);
        ret = fwrite(&x, sizeof(info *), 1, arq);
        // if (ret)
        //     printf("Gravou %d valores com sucesso!\n", ret);
        // else
        //     printf("Erro ao gravar...\n");
    }
    fclose(arq);
    return 0;
}
