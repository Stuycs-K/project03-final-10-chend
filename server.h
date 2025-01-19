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
  int init1;
  int init2;

};
static int returnplayer1choice(int player1choice);



struct message {
  int setindex; //index for player
  int setindexopponent; //index for opponent
  int value; //for rock paper scissors
  char servermsg[256]; //pass means wait for players
		   //go means ask for user input/player sent msg to server(go1/go2)
		   //win means player won, wait for next 
		   //lose means player lost, SIGINT
	           //draw, play against each other again
		   //disconnect means player disconnected
};



void printconnections(struct connection** listofconnections2);
int generateindex();
int* insertionsort(int* listofindices, int size);
struct connection** sortconnections(struct connection** listofconnections2);
int mainserversetup();
int serverconnect(int* fromPlayer);
int remakeconnections(struct connection** listofconnections2);
static void ignore_SIGPIPE(int signum);
