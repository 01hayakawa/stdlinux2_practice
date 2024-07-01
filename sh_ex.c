#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFF_SIZE 1024

static void exec_command(char *command, int in_fd, int out_fd);

int main(int argc, char *argv[]) {
  // 標準入力からコマンドを得る
  char buf[BUFF_SIZE];
  if (fgets(buf, BUFF_SIZE, stdin) == NULL) {
    exit(0);
  }

  // 末尾の改行文字を取り除く
  int len = strlen(buf);
  if (buf[len - 1] == '\n') {
    buf[len - 1] = '\0';
  }

  // コマンドをパイプで分離する
  char *commands[BUFF_SIZE];
  int n_pipe = 0;
  char *ptr = strtok(buf, "|");
  while (ptr != NULL) {
    commands[n_pipe++] = ptr;
    ptr = strtok(NULL, "|");
  }

  int in_fd = 0; // 最初のコマンドは標準入力から読み取る

  for (int i = 0; i < n_pipe; i++) {
    int pipefd[2];
    if (i < n_pipe - 1) { // 最後のコマンドでない場合はパイプを作成
      if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
      }
    } else {  // 最後のコマンドは標準出力に出力
      pipefd[0] = 0;
      pipefd[1] = 1;
    }

    exec_command(commands[i], in_fd, pipefd[1]);

    if (in_fd != 0) {
      close(in_fd); // 使い終わった読み取り用ファイルディスクリプタを閉じる
    }
    if (pipefd[1] != 1) {
      close(pipefd[1]); // 書き込み用ファイルディスクリプタを閉じる
    }

    in_fd = pipefd[0]; // 次のコマンドの入力はこのコマンドの出力
  }
  // 最後の子プロセスの終了を待つ
  for (int i = 0; i < n_pipe; i++) {
    wait(NULL);
  }

  return 0;
}

static void exec_command(char *input, int in_fd, int out_fd) {
  // 引数の数を数える
  int n_args = 0;
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == ' ') n_args++;
  }

  // argsを用意する
  char *args[n_args + 2];

  // コマンドと引数を分離する
  char *ptr;
  ptr = strtok(input, " ");
  int command_len = strlen(ptr);
  char command[command_len + 1];
  strcpy(command, ptr);

  args[0] = command;

  int j = 1;
  while (ptr != NULL) {
    ptr = strtok(NULL, " ");
    if (ptr != NULL) {
      args[j] = ptr;
      j++;
    }
  }
  args[j] = NULL;

  // 子プロセスを作る
  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    exit(1);
  } else if (pid == 0) {
    if (in_fd != 0) {
      dup2(in_fd, STDIN_FILENO);
      close(in_fd);
    }
    if (out_fd != 1) {
      dup2(out_fd, STDOUT_FILENO);
      close(out_fd);
    }

    execvp(command, args);
    perror(command);
    exit(99);
  }
}
