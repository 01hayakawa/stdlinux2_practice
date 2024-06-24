#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <getopt.h>

static struct option longopts[] = {
  {"tab", no_argument, NULL, 't'},
  {"newline", no_argument, NULL,'n'},
  {0,0,0,0}
};

int
main(int argc, char *argv[])
{
  int i;
  int nopt = 0;
  int topt = 0;
  int opt;

  while ((opt = getopt_long(argc, argv, "nt", longopts, NULL)) != -1) {
    switch (opt) {
      case 'n':
        nopt = 1;
        break;
      case 't':
        topt = 1;
        break;
      default:
        printf("error! \'%c\' \'%c\'\n", opt, optopt);
        return 1;
    }
  }

  for(i = optind; i < argc; i++){
    FILE *f;
    int c;

    f =fopen(argv[i], "r");
    if(!f){
      perror(argv[i]);
      exit(1);
    }
    while ((c = fgetc(f)) != EOF){
        if (topt == 1 && c == '\t'){
          fputs("\\t",stdout);
        }else if (nopt == 1 && c == '\n'){
          fputs("\\n",stdout);
        }
        else if (putchar(c) < 0) exit(1);
    }
    fclose(f);
  }
  exit(0);
}