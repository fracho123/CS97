/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <errno.h>
//#include <immintrin.h>
//#include <limits.h>
#include <stdbool.h>
//#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include "rand64-hw.h"
#include "rand64-sw.h"
#include "options.h"
#include "output.h"


/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */

  struct options opt;
  checkargs (argc, argv, &opt);
  if (!opt.valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  struct drand48_data buf = {0}; // initialized once to be used for rng
  void mrand48_rng_init(void)
    {
    }
  unsigned long long mrand48_rng(void)
    {
      long int a, b;
      mrand48_r(&buf, &a);
      mrand48_r(&buf, &b);
      return (((unsigned long long) a) << 32) | ((unsigned long long) b & 0x00000000FFFFFFFF);
    }
  void mrand48_rng_fini(void)
    {
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (opt.nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  //void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
  
  if (opt.type_rng==RD && !rdrand_supported ())
    {
      fprintf (stderr, "hardware random-number generation is not available. \n");
      exit(1);
    }
  else if (opt.type_rng==RD && rdrand_supported ())
    {
      hardware_rand64_init();
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
  else if (opt.type_rng==SF)
    {
      software_rand64_init(opt.r_src);
      rand64 = software_rand64;
      finalize = software_rand64_fini;
    }
  else if (opt.type_rng==MR)
    {
      mrand48_rng_init();
      rand64 = mrand48_rng;
      finalize = mrand48_rng_fini;
    }

  //initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  unsigned int block;
  int KiB = 1024;

  unsigned long long x = rand64 ();
  int outbytes;
  if (opt.block_size != 0) {
      x = 0;
      block = KiB * opt.block_size;
      char* buffer = (char*) malloc(block);
      if (buffer == NULL) {
          fprintf (stderr, "Error: null memory. \n");
          exit(1);
      }
      do {
          outbytes = opt.nbytes < block ? opt.nbytes : block;
          for (int i = 0; i < outbytes; i++) {
              if (i%8==0)
                x=rand64();
              buffer[i] = x >> (i%8);
          }
          write(1, buffer, outbytes);
          opt.nbytes -= outbytes;
        }
      while (0 < opt.nbytes);
  }
  else {
      do {
          outbytes = opt.nbytes < wordsize ? opt.nbytes : wordsize;
          if (!writebytes (x, outbytes))
            {
              output_errno = errno;
              break;
            }
          opt.nbytes -= outbytes;
        }
      while (0 < opt.nbytes);
  }

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
}
