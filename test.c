#include <stdio.h>

int
main(int argc,char *argv[])
{

  for (int i = 0; i < 5; i++){
    printf("%c\n",ary[i]);
  }

    for (int i = 0; i < 5; i++){
    printf("%c\n",*(ary + i));
  }

}
