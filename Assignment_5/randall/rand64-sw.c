//#include <errno.h>
//#include <immintrin.h>
//#include <limits.h>
//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "rand64-sw.h"

/* Software implementation.  */

/* Input stream containing random bytes.  */
FILE *urandstream;

/* Initialize the software rand64 implementation.  */
void
software_rand64_init (char* r_src)
{
  if (*r_src == '\0') {
      urandstream = fopen ("/dev/random", "r");
  }
  else {
      urandstream = fopen (r_src, "r");
  }

  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void)
{
  fclose (urandstream);
}

