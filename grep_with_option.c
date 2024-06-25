#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <getopt.h>

static struct option longopts[] = {
  {"ignore-case", no_argument, NULL, 'i'},
  {"invert-match", no_argument, NULL,'v'},
  {0,0,0,0}
};

static void do_grep(regex_t *pat, FILE *f, int iopt, int vopt);

int main(int argc, char *argv[])
{
  int iopt = 0;
  int vopt = 0;
  int opt;

  while ((opt = getopt_long(argc, argv, "iv", longopts, NULL)) != -1) {
    switch (opt) {
      case 'i':
        iopt = 1;
        break;
      case 'v':
        vopt = 1;
        break;
      default:
        printf("error! \'%c\' \'%c\'\n", opt, optopt);
        return 1;
    }
  }

  regex_t pat;
  int err;
  int i;

  if (argc < 2) {
    fputs("no pattern\n", stderr);
    exit(1);
  }
  err = regcomp(&pat, argv[optind], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
  if (err != 0){
    char buf[1024];
    regerror(err, &pat, buf, sizeof buf);
    puts(buf);
    exit(1);
  }
  if (argc == 2){
    do_grep(&pat, stdin, iopt, vopt);
  }
  else{
    for (i = optind +1 ; i < argc; i++){
      FILE *f;
      f = fopen(argv[i],"r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_grep(&pat, f, iopt, vopt);
      fclose(f);
    }
  }
  regfree(&pat);
  exit(0);
}

static void
do_grep(regex_t *pat, FILE *src,int iopt,int vopt) // オプション受け取る
{
  char buf[4096];

  while (fgets(buf,sizeof buf, src)) {
    if (vopt == 1){
      if (regexec(pat, buf, 0, NULL, 0) != 0){
        fputs(buf,stdout);
      }
    } else {
      if (regexec(pat, buf, 0, NULL, 0) == 0){
        fputs(buf,stdout);
      }
    }
  }
}