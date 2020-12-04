#ifndef OPTIONS_HEADER
#define OPTIONS_HEADER

//#include <errno.h>
//#include <immintrin.h>
//#include <limits.h>
//#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

enum Method { RD, MR, SF };
enum Write { STD, N };

struct options {
    bool valid; // checking if input is valid
    long long nbytes; // number of bytes to produce                                   
    enum Method type_rng; // way to generate the random sequence
    char* r_src; // string that holds file path if necessary
    enum Write output; // where to output data
    int block_size; // outputted buffer size in kB
};

void
checkargs (int argc, char **argv, struct options *opt); 

#endif
