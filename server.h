#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>


struct connection {
  //file desc representing a single round, init1 and init2 are for checking if a player stays online while the server waits for players
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
  char servermsg[256]; 
		   //pass means wait for players/opponent
		   //go1 means ask for user input/player, send result to server(player1)
		   //go2 means ask for user input/player, send result and who won to server(player2)
		   //won means player won, wait for next 
		   //wonall means player won the tournament, all players and server should quit
		   //lose means player lost, SIGINT
	           //draw, play against each other again
		   //QUIT means player disconnected
};



void printconnections(struct connection** listofconnections2);
int generateindex();
int* insertionsort(int* listofindices, int size);
struct connection** sortconnections(struct connection** listofconnections2);
int mainserversetup();
int serverconnect(int* fromPlayer);
int remakeconnections(struct connection** listofconnections2);
static void ignore_SIGPIPE(int signum);
