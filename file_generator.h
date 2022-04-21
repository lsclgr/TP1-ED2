#ifndef file_generator_h
    #define file_generator_h

    #include <stdlib.h>
    
    #include "indexed_sequential_access.h"

    void test_file(FILE *fp);
    int rand_num_by_max(int max);
    void create_string(int max_size, char str);
    void build_file(int nPages, int page_size, int data_max, int key_dif);
#endif