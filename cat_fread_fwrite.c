#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 20

int
main(int argc, char *argv[])
{
  int i;

  for(i = 1; i < argc; i++){
    FILE *f;

    f =fopen(argv[i], "r");
    if(!f){
      perror(argv[i]);
      exit(1);
    }
    size_t ret;
    char buf[BUFFER_SIZE];
    for(;;){
      memset(buf, '\0', sizeof(buf));
      ret = fread(buf, 1, BUFFER_SIZE, f);
      printf("%s", buf);
      if (ret < BUFFER_SIZE){
        fclose(f);
        exit(1);
      }
    }
  }
  exit(0);
}