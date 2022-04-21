#include <stdio.h>
#include <stdlib.h> /* Para a função exit() */
#include <time.h>

typedef struct {
    int chave;
    long long int dado1;
    char *dado2;
} info;
void gen_random(char *s, const int len, int timex) {
    srand(timex);
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len - 1; i++) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len - 1] = '\0';
}

int main() {
    FILE *arq;
    int timex = time(NULL);
    
    arq = fopen("arquivo.bin", "wb+");
    if (!arq) {
        printf("Erro na abertura do arquivo. Fim de programa.");
        exit(1);
    }
    int len = 10, tam = 5, ret;
    info x;

    for (int i = 0; i < tam; i++) {
        srand(timex);
        x.chave = i;
        x.dado1 = rand() % 2147483647;
        printf("%lld\n", x.dado1);
        x.dado2 = (char *)malloc(len * sizeof(char));
        gen_random(x.dado2, len, timex);
        ret = fwrite(&x, sizeof(info), 1, arq);
        printf("%d Salvo\n",ret);
        timex++;
    }
    fclose(arq);

    arq = fopen("arquivo.bin", "rb+");
    if (!arq) {
        printf("Erro na abertura do arquivo. Fim de programa.");
        exit(1);
    }
    info opa;

    for (int i = 0; i < tam; i++) {
        ret = fread(&opa,sizeof(info),1,arq);
        printf("%d\n", ret);

        printf("\n%d\n%lld\n%s\n",opa.chave,opa.dado1,opa.dado2);
    }
    fclose(arq);
    return 0;
}
