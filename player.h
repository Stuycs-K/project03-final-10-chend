#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>


static void quit(int signum);
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
static int returnfromPlayer(int fromPlayer);
static int returnindex(int index);
int random_index();
int playerhandshake(int* fromPlayer);
static void ignore_SIGPIPE(int signum);
