#ifndef indexed_sequential_acess_h
    #define indexed_sequential_acess_h

    #define PAGE_SIZE 4

    typedef struct {
        int key;
        int data;
    } Item;

    typedef struct {
        Item arr[PAGE_SIZE];
    } Page;

    typedef struct {
        
    } Index;
#endif
