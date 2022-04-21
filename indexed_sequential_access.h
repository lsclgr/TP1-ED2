#ifndef indexed_sequential_acess_h
    #define indexed_sequential_acess_h

    #include <limits.h>

    #define PAGE_SIZE 4
    #define DATA1_MAX LONG_MAX
    #define DATA2_SIZE 5000
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
#endif
