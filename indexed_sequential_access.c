#include <stdio.h>
#include <stdlib.h>

#include "indexed_sequential_access.h"
#include "file_generator.h"

void createIndex(Index *pIndex)
{
    FILE fp = fopen("data.bin", "rb");
    test_file(fp);

    fread(&pIndex->nPages, sizeof(int), 1, fp);
    fread(&pIndex->arr[0], sizeof(Key), 1, fp);
    for(int i = 0; i < (pIndex->nPages - 1); i++)
    {
        fseek(fp, (PAGE_SIZE - 1) * sizeof(Item), SEEK_CUR);
        fread(&pIndex->arr[i], sizeof(Key), 1, fp);
    }

    fclose(fp);
}

int main()
{

}

