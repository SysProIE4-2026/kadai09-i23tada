/*
 *  myenv.c : env コマンドのクローン
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef __linux__
#include "myputenv.h"
#endif

int main(int argc, char* argv[], char* envp[]) {
  int i;
  // 引数の環境変数を設定
  for (i = 1; i < argc; i++) {
    // コマンドがあれば(環境引数の形式じゃなければ)ループを抜ける
    if (strchr(argv[i], '=') == NULL) {
      break;
    }
    // 環境変数を設定
    // コマンドの場合は上で処理できている
    if (putenv(argv[i]) != 0) {
      perror("putenv");
      return 1;
    }
  }

  // 環境引数だけだと上のforループでi>=argc
  // その場合はprintenvを実行する
  // コマンドがあればそのコマンドを実行する
  if (i >= argc) {
    char* cmd[] = {"printenv", NULL};
    execvp(cmd[0], cmd);
    perror("execvp");
    return 1;
  } else {
    // &で配列の途中から最後まで全てを含めたコマンドを実行できる
    execvp(argv[i], &argv[i]);
    perror("execvp");
    return 1;
  }
}

/* 実行例
・コンパイルエラーチェック
$ ls
Makefile  myenv.c  myputenv.c*  myputenv.h  README.md  README.pdf
$ make myenv
cc -D_GNU_SOURCE -Wall -std=c99 -o myenv myenv.c myputenv.c
$ ls
Makefile  myenv*  myenv.c  myputenv.c*  myputenv.h  README.md  README.pdf

・環境変数が変更できる
LANGを初期値のja_JP.UTF-8からen_GB.UTF-8に変更
$ ./myenv | grep -E '^LANG='
LANG=ja_JP.UTF-8
$ ./myenv LANG=en_GB.UTF-8 | grep -E '^LANG='
LANG=en_GB.UTF-8

・複数の環境変数を変更できる
$ ./myenv LC_TIME=ja_JP.UTF-8 TZ=Europe/London | grep -E '^LC_TIME|^TZ'
LC_TIME=ja_JP.UTF-8
TZ=Europe/London

$ ./myenv LC_TIME=ja_JP.UTF-8 TZ=Europe/London date
2026年 6月30日 火曜日 03時58分57秒 BST


$ ./myenv LC_TIME=en_GB.UTF-8 TZ=Cuba | grep -E '^LC_TIME|^TZ'
LC_TIME=en_GB.UTF-8
TZ=Cuba

$ ./myenv LC_TIME=en_GB.UTF-8 TZ=Cuba date
Mon 29 Jun 2026 22:59:11 CDT

・引数つきのコマンド
$ ./myenv LC_TIME=en_GB.UTF-8 ls -l
total 384
-rw-r--r--  1 tata  staff     132 30 Jun 10:37 Makefile
-rwxr-xr-x  1 tata  staff   33784 30 Jun 11:49 myenv
-rw-r--r--  1 tata  staff    2037 30 Jun 21:36 myenv.c
-rwxr-xr-x  1 tata  staff     216 30 Jun 10:37 myputenv.c
-rw-r--r--  1 tata  staff      93 30 Jun 10:37 myputenv.h
-rw-r--r--  1 tata  staff    1268 30 Jun 10:37 README.md
-rw-r--r--  1 tata  staff  139098 30 Jun 10:37 README.pdf

*/
