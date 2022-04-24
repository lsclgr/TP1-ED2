#ifndef file_generator_h
    #define file_generator_h

    #include <stdlib.h>
    
    #include "indexed_sequential_access.h"

    #define N_PAGES 100 / PAGE_SIZE
    #define FILE_MAX_SIZE (N_PAGES * PAGE_SIZE) + 1

    void test_file(FILE *fp);
    long int rand_num_by_max(long int max);
    void create_string(int max_size, char str[]);
    void build_file(int nPages, int page_size, long int data_max, int key_dif);
#endif