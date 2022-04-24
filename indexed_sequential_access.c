#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "indexed_sequential_access.h"
#include "file_generator.h"

void isam_create_index(Index *pIndex)
{
    FILE *fp = fopen("data.bin", "rb");
    test_file(fp);

    fread(&pIndex->nPages, sizeof(int), 1, fp); 
    for(int i = 0; i < pIndex->nPages; i++)
    {
        fseek(fp, (sizeof(Page) * i) + sizeof(int), SEEK_SET);
        fread(&pIndex->arr[i], sizeof(Key), 1, fp);
    }

    fclose(fp);
}

Item* isam_binary_search(Key key, Page *page, int l, int r, int type)
{
    int m = (l + r) / 2;

    if(page->arr[m].key != key && l >= r)
        return NULL;
    else if(page->arr[m].key == key)
        return &page->arr[m];
    else if((page->arr[m].key > key && type == 1) || (page->arr[m].key < key && type == 2))
        return isam_binary_search(key, page, l, m - 1, type);
    else
        return isam_binary_search(key, page, m + 1, r, type);
}

/*
Item* isam_desc_binary_search(Key key, Page page, int l, int r)
{
    int m = (l + r) / 2;

    if(page.arr[m].key != key && l == r)
        return NULL;
    else if(page.arr[m].key == key)
        return &page.arr[m];
    else if(page.arr[m].key > key)
        return binary_search(key, page, l, m - 1);
    else
        return binary_search(key, page, m + 1, r);
}
*/

Item* isam_linear_search(Key key, Page *page, int n_items)
{
    for(int i = 0; i < n_items; i++)
        if(page->arr[i].key == key)
            return &page->arr[i];
    
    return NULL;
}

void isam_copy_item(Item *i1, Item *i2)
{
    i2->data1 = i1->data1;
    i2->key = i1->key;
    strcpy(i2->data2, i1->data2);
}

bool isam_item_search(Key key, Item *pItem, Index *pIndex, int status)
{
    int i = 0;
    while(i < pIndex->nPages && key >= pIndex->arr[i]) i++;
    
    if(i == 0) return false;

    FILE *fp = fopen("data.bin", "rb");
    test_file(fp);

    Page page;
    int n_items;

    fseek(fp, PAGE_SIZE * (i - 1) * sizeof(Item) + sizeof(int), SEEK_SET);
    if(i != pIndex->nPages)
    {
        fread(page.arr, sizeof(Page), 1, fp);
        n_items = PAGE_SIZE;
    }
    else
    {
        fseek(fp, 0, SEEK_END);
        int item_total_qtt = ftell(fp) / sizeof(Item);
        n_items = item_total_qtt % PAGE_SIZE;
        fread(page.arr, sizeof(Item), n_items, fp);
    }

    fclose(fp);

    Item *item_found;
    if(status == 1 || status == 2)
        item_found = isam_binary_search(key, &page, 0, n_items - 1, status);
    else
        item_found = isam_linear_search(key, &page, n_items);

    if(item_found)
    {
        isam_copy_item(item_found, pItem);
        return true;
    }
    else
    {
        return false;
    }
}

void isam_auto_search(Index *pIndex, int file_type)
{
    int key_max = pIndex->arr[N_PAGES - 1];

    for(int i = 0; i < 20; i++)
    {
        Item item;
        Key key_search = rand_num_by_max(key_max);
        if(isam_item_search(key_search, &item, pIndex, file_type))
        {
            printf("Item %d was found!\n", item.key);
            printf("d1 = %ld\n", item.data1);
            printf("d2 = %s\n\n", item.data2);
        }
        else
        {
            printf("Item %d not found...\n\n", key_search);
        }
    }
}

int main()
{
    srand(time(NULL));

    build_file(N_PAGES, PAGE_SIZE, DATA1_MAX, PAGE_SIZE);

    Index index;
    index.arr = (Key*) malloc(N_PAGES * sizeof(Key));
    index.nPages = N_PAGES;
    isam_create_index(&index);

    int file_type = 1;
    isam_auto_search(&index, file_type);

    free(index.arr);

    return 0;
}

