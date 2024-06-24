#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

static void count_line(const char *path);
static void die(const char *s);

int
main(int argc,char *argv[])
{
  int i;
  if (argc < 2) {
    fprintf(stderr, "%s: file name not given\n",argv[0]);
      exit(1);
  }
  for (i = 1; i < argc; i++) {
    count_line(argv[i]);
  }
  exit(0);
}

#define BUFFER_SIZE 2048

static void
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
  printf("%d\n",count);
  if (fclose(stream) < 0) die (path);
}

static void
die(const char *s)
{
  perror(s);
  exit(1);
}
