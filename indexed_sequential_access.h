#ifndef indexed_sequential_acess_h
    #define indexed_sequential_acess_h

    #define PAGE_SIZE 4

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
#endif
