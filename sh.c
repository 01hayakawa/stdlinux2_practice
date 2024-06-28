#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFF_SIZE 1024
int
main(int argc, char *argv[])
{
  // 標準入力からコマンドを得る
  char buf[BUFF_SIZE];
  if(fgets(buf, BUFF_SIZE, stdin) == NULL){
    exit(0);
  }

  // 引数の数を数える
  int n_args = 0;
  for (int i = 0; i < strlen(buf); i++){
    if (buf[i] == ' ') n_args++;
  }

  // argsを用意する
  char *args[n_args + 2];

  // コマンドと引数を分離する
  char *ptr;
  ptr = strtok(buf, " ");
  int command_len = strlen(ptr);
  char command[command_len];
  strcpy(command, ptr);

  if (command[command_len - 1] == '\n'){
    command[command_len -1]= '\0';
  }
  args[0] = command;

  int j = 1;
  while(ptr != NULL) {
    // strtok関数により変更されたNULLのポインタが先頭
    ptr = strtok(NULL, " ");

    // ptrがNULLの場合エラーが発生するので対処
    if (ptr != NULL) {
      int len = strlen(ptr);
      if (ptr[len - 1] == '\n'){
        ptr[len - 1] = '\0';
      }
        args[j] = ptr;
        j++;
    }
  }
  args[n_args + 1] = NULL;

  // 子プロセスを作る
  pid_t pid;
  pid = fork();
  // コマンドを実行する(子プロセス)
  if (pid == 0){
    execvp(command, args);
    perror(command);
    exit(99);
  }
  else{  // 子プロセスの終了を待つ(親プロセス)
    int status;
    waitpid(pid, &status, 0);
  }
}