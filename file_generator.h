#ifndef file_generator_h
    #define file_generator_h

    #include <stdlib.h>
    #include <limits.h>

    #define DATA1_MAX LONG_MAX
    #define DATA2_SIZE 5000
    #define KEY_DIF 5

    void test_file(FILE *fp);
    int rand_num_by_max(int max);
    void createString(int max_size, char str);
    void build_file(int nPages, int page_size, int data_max, int key_dif);
#endif