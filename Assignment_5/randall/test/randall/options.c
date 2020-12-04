#include <errno.h>
//#include <immintrin.h>
//#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include "options.h"

bool
isPositiveInteger(char* letter) {
  int i;
  for (i=0; letter[i] != '\0'; i++) {
    if (!isdigit(letter[i]))
      return false;
  }
  return true;
}

void
checkargs (int argc, char **argv, struct options *opt)
{
  /* Check arguments.  */
  if (argc >= 2)
    {
      char *endptr;
      errno = 0;
      opt->nbytes = strtoll (argv[argc-1], &endptr, 10);
      if (errno)
	        perror (argv[1]);
      else
	  opt->valid = !*endptr && 0 <= opt->nbytes;
    }

  int opt2 = 0;
  bool optioni = false;
  bool optione = false;
  while ((opt2 = getopt(argc, argv, "i:o:")) != -1) {
      char first = optarg[0];
      switch (opt2) {
      case 'i':
          optioni = true;
          if (first == '/') {
              opt->r_src = optarg;
              opt->type_rng=SF;
          }
          else if (strcmp("rdrand", optarg) == 0)
              opt->type_rng=RD;
          else if (strcmp("mrand48_r", optarg) == 0)
              opt->type_rng=MR;
          else {
              fprintf (stderr, "Improper -i input.\n");
              exit(1);
          }
          break;

      case 'o':
          optione = true;
          if (strcmp("stdio", optarg) == 0)
              opt->output=STD;
          else if (isPositiveInteger(optarg)) {
              opt->output=N;
              opt->block_size=atoi(optarg);
              if (opt->block_size <= 0) {
                  fprintf (stderr, "Not a positive integer. \n");
                  exit(1);
              }
          }
          else {
              fprintf (stderr, "Improper -o output.\n");
              exit(1);
          }
          break;
      case '?':
          fprintf (stderr, "Improper option.\n");
          exit(1);
      default:
          break;
      }
  }

  if (!optioni) {
      opt->type_rng=RD;
  }
  if (!optione) {
      opt->output=STD;
  }
}
