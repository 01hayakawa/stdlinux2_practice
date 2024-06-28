#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

static void mkdir_traverse(const char* current_dir, const char *path);

int
main(int argc,char *argv[])
{
  int i;

  if (argc < 2){
    fprintf(stderr, "%s: no argument\n", argv[0]);
  }
  for (i = 1; i < argc; i++){
    mkdir_traverse(".", argv[i]);
  }
  exit(0);
}

static void mkdir_traverse(const char *current_dir, const char *path)
{
  char dir_name[1024] = "";
  char rest_of_path[1024] = "";
  char *buf = dir_name;

  for (int i = 0; i < strlen(path); i++){
    if (path[i] == '/'){
      buf = rest_of_path;
    }
    size_t len = strlen(buf);
    buf[len] = path[i];
    buf[len + 1] = '\0';
  }

  char target_dir[2048];
  snprintf(target_dir, sizeof(target_dir), "%s/%s", current_dir, dir_name);
  if (mkdir(target_dir,0777) < 0) {
    perror(target_dir);
    exit(1);
  }
  printf("make %s\n", target_dir);

  if (rest_of_path[0] != '\0') mkdir_traverse(target_dir, rest_of_path + 1);
}


