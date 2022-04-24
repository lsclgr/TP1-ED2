#ifndef indexed_sequential_acess_h
    #define indexed_sequential_acess_h

    #include <limits.h>
    #include <stdbool.h>

    #define PAGE_SIZE 4
    #define DATA1_MAX LONG_MAX
    #define DATA2_SIZE 64
    #define KEY_DIF 5

    typedef int Key;

    typedef struct {
        Key key;
        long int data1;
        char data2[DATA2_SIZE + 1];
    } Item;

    typedef struct {
        Item arr[PAGE_SIZE];
    } Page;

    typedef struct {
        Key *arr;
        int nPages;
    } Index;

    void create_index(Index *pIndex);
    Item* isam_binary_search(Key key, Page *page, int l, int r, int type);
    Item* isam_linear_search(Key key, Page *page, int n_items);
    void isam_copy_item(Item *i1, Item *i2);
    bool isam_item_search(Key key, Item *pItem, Index *pIndex, int status);
#endif
