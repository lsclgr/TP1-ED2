#ifndef file_generator_h
    #define file_generator_h

    #include <stdlib.h>

    #define DATA_MAX 100
    #define KEY_DIF 5

    void test_file(FILE *fp);
    int rand_num_by_max(int max);
    void build_file(int nPages, int page_size, int data_max, int key_dif);
#endif