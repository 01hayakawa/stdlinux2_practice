#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

static int count_line(const char *path);
static void die(const char *s);
static void do_tail(const char *path, const int nlines);

int
main(int argc,char *argv[])
{
  int i;
  if (argc < 2) {
    fprintf(stderr, "%s: file name not given\n",argv[0]);
      exit(1);
  }
  for (i = 1; i < argc; i++) {
    int nlines = count_line(argv[i]);
    do_tail(argv[i], nlines);
  }
  exit(0);
}

#define BUFFER_SIZE 2048

static int
count_line(const char *path)
{
  char buf[BUFFER_SIZE];
  FILE *stream = fopen(path, "r");
  if (stream == NULL) die(path);

  int count = 1;
  char *ret;
  for(;;) {
    ret = fgets(buf, 10, stream);
    if (ferror(stream)) die(path);
    if (ret == NULL) {
      break;
    }
    else {
      if(buf[strlen(buf) -1] == '\n') count ++;
    }
  }
  if (fclose(stream) < 0) die (path);
  return count;

}

#define TAIL_SIZE 2

static void
do_tail(const char *path,const int nlines)
{
  FILE *f;
    int c;

    f =fopen(path, "r");
    if(!f){
      perror(path);
      exit(1);
    }

    int nskip;
    if (nlines <= TAIL_SIZE){
      nskip = 0;
    }else{
      nskip = nlines - TAIL_SIZE;
    }

    while (nskip != 0){
        c = fgetc(f);
        if (c == '\n'){
          nskip--;
        }
    }

    while ((c = fgetc(f)) != EOF){
        if (putchar(c) < 0) exit(1);
    }

    fclose(f);
}


static void
die(const char *s)
{
  perror(s);
  exit(1);
}
