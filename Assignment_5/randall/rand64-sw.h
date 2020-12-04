#ifndef SW_HEADER
#define SW_HEADER

//#include <errno.h>
//#include <immintrin.h>
//#include <limits.h>
//#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>


/* Software implementation.  */

/* Input stream containing random bytes.  */
//FILE *urandstream;

/* Initialize the software rand64 implementation.  */
void
software_rand64_init (char* r_src);

/* Return a random value, using software operations.  */
unsigned long long
software_rand64 (void);

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void);

#endif
