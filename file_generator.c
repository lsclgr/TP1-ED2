#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "file_generator.h"
#include "indexed_sequential_access.h"

void test_file(FILE *fp)
{
    if(fp == NULL)
    {
        fprintf(stderr, "\nError: File cannot be opened...\n");
        exit(1);
    }
}

// Item item_constructor(int data, int key);

int rand_num_by_max(int max)
{
    return 1 + rand() % max;
}

void createString(int max_size, char str[])
{
    int size = rand_num_by_max(max_size);
    for(int i = 0; i < size; i++)
        str[i] = rand() % CHAR_MAX;
}

void build_file(int nPages, int page_size, int data_max, int key_dif) // creates a file with random data
{
    FILE *fp = fopen("data.bin", "wb");
    test_file(fp);

    fwrite(&nPages, sizeof(int), 1, fp);

    Page page;
    page.arr[page_size - 1].key = 0;
    for(int i = 0; i < (nPages - 1); i++)
    {
        page.arr[0].key = page.arr[page_size - 1].key + rand_num_by_max(key_dif);
        page.arr[0].data1 = rand_num_by_max(data_max);
        createString(page.arr[0].data2);
        for(int j = 1; j < page_size; j++)
        {
            page.arr[j].data1 = rand_num_by_max(data_max);
            createString(page.arr[j].data2);
            page.arr[j].key = page.arr[j - 1].key + rand_num_by_max(key_dif);
        }

        fwrite(&page, sizeof(Page), 1, fp);
    }

    int last_page_size = 1 + rand() % page_size;
    for(int i = 0; i < last_page_size; i++)
    {
        Item item;
        item.data1 = rand_num_by_max(data_max);
        createString(item.data2);
        item.key = rand_num_by_max(key_dif);

        fwrite(&item, sizeof(Item), 1, fp);
    }

    fclose(fp);
}