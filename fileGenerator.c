#include <limits.h>
#include <stdio.h>
#include <stdlib.h> /* Para a função exit() */
#include <time.h>

typedef struct {
    int key;
    long int data1;
    char *data2;
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
    int len = 5000, tam = 200000, ret, op = 0, keys[tam];
    info x;
    switch (op) {
        case 0:
            for (int i = 1; i <= tam; i++) {
                srand(timex);
                x.key = i;
                x.data1 = rand() % LONG_MAX;
                // printf("%lld\n", x.data1);
                x.data2 = (char *)malloc(len * sizeof(char));
                gen_random(x.data2, len, timex);
                ret = fwrite(&x, sizeof(info), 1, arq);
                // printf("salvo");
                timex++;
            }
            break;
        case 1:
            for (int i = tam; i > 0; i--) {
                srand(timex);
                x.key = i;
                x.data1 = rand() % LONG_MAX;
                // printf("%lld\n", x.data1);
                x.data2 = (char *)malloc(len * sizeof(char));
                gen_random(x.data2, len, timex);
                ret = fwrite(&x, sizeof(info), 1, arq);
                // printf("salvo");
                timex++;
            }
            break;
        case 2:
            for (int i = 0; i < tam; i++) {
                srand(timex);
                x.key = rand() % INT_MAX;
                int verify = 1;
                while (verify) {
                    verify = 0;
                    for (int j = 0; j < i; i++) {
                        if (keys[j] == x.key) {
                            verify = 1;
                        }
                    }
                    x.key = rand() % INT_MAX;
                }
                keys[i] = x.key;
                x.data1 = rand() % LONG_MAX;
                // printf("%lld\n", x.data1);
                x.data2 = (char *)malloc(len * sizeof(char));
                gen_random(x.data2, len, timex);
                ret = fwrite(&x, sizeof(info), 1, arq);
                // printf("salvo");
                timex++;
            }
            break;

        default:
            break;
    }

    fclose(arq);

    return 0;
}
