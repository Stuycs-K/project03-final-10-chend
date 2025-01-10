#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
union semun {
  int val;                  //used for SETVAL
  struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
  unsigned short  *array;   //used for SETALL
  struct seminfo  *__buf;
};


struct sembuf {
  short sem_op;
  short sem_num;
  short sem_flag;
};

struct connection {
  int toPlayer1;
  int fromPlayer1;
  int indexPlayer1;
  int toPlayer2;
  int fromPlayer2;
  int indexPlayer2;
  int toWinner;
  int fromWinner;

}


int mainserversetup();
int serverconnect(int* fromPlayer);

static void ignore_SIGPIPE(int signum);
