#include "server.h"



int main(){
	signal(SIGPIPE, ignore_SIGPIPE);
	int** listofconnections = malloc(sizeof(int) * 12);
	for(int i = 0; i < 6; i ++){
			//array for file desc of 2 players
			int* connection = malloc(sizeof(int) * 2);
			connection[0] = -1;
			connection[1] = -1;
			listofconnections[i] = connection;
	}
	while(1){
		int toPlayer;
		int fromPlayer;
		fromPlayer = mainserversetup();
		//if(fork() == 0){
			
			//toPlayer = serverconnect(&fromPlayer);
			//printf("Player connected to server! \n");
		//}

		//once connected to a player, find the first valid person to play against
		//will be using half of the array for the 2nd round(wait for their round to finish)
		for(int i = 0; i < 6; i ++){
			
			
		}



	}

}





int mainserversetup(){
	int fromPlayer;
	mkfifo("PlayerToServer", 0666);
	
	fromPlayer = open("PlayerToServer", O_RDONLY, 0);
	
	remove("PlayerToServer");
	return fromPlayer;
	
}


int serverconnect(int* fromPlayer){
	int playerindex;
	read(*fromPlayer, &playerindex, sizeof(int));
	//printf("SERVER RECIEVED INDEX: %d \n", playerindex);
	int toPlayer;
	toPlayer = open("ServerToPlayer", O_WRONLY, 0);
	int playerindex2 = playerindex + 1;
	write(toPlayer, &playerindex2, sizeof(int));
	return toPlayer;
	

}


static void ignore_SIGPIPE(int signum){
	
		

}

