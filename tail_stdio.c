#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define _GNU_SOURCE
#include <getopt.h>

#define BUFFER_SIZE 1024
#define TAIL_SIZE 2

static void do_tail(const char input[][BUFFER_SIZE], const int nlines,const long tailsize);

static struct option longopts[] = {
  {"lines", required_argument, NULL, 'n'},
  {0,0,0,0}
};

int
main(int argc,char *argv[])
{
  int opt;
  long tailsize = TAIL_SIZE;
  while ((opt = getopt_long(argc, argv , "n:", longopts, NULL)) != -1) {
    switch (opt){
    case 'n':
      tailsize = atol(optarg);
      break;
    case '?':
      fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
      exit(1);
    }
  }
  int i = 0;
  int nlines = 0;
  char input_line[BUFFER_SIZE];
  char input_all[BUFFER_SIZE][BUFFER_SIZE];

  while (fgets(input_line, BUFFER_SIZE, stdin) !=NULL){
    strcpy(input_all[i], input_line);
    nlines++;
    i++;
  }
  do_tail(input_all, nlines, tailsize);
  exit(0);
}

static void
do_tail(const char input[][BUFFER_SIZE], const int nlines, const long tailsize)
{
  int i;
  int nskip;
  if (nlines <= tailsize){
    nskip = 0;
  }else{
    nskip = nlines - tailsize;
  }
  for (int i = nskip; i < nlines; i++) {
    fputs(input[i], stdout);
  }
}