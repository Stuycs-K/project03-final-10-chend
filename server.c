#include "server.h"



int main(){
	signal(SIGPIPE, ignore_SIGPIPE);
	while(1){
		int toPlayer;
		int fromPlayer;
		fromPlayer = mainserversetup();
		if(fork() == 0){

			toPlayer = serverconnect(fromPlayer);
			printf("Player connected to server! \n");
		}



	}

}


int** sort(int* unsorted){
	//[[], [], []] format, each subarray will contain 2 indices


}


int mainserversetup(){
	int fromPlayer;
	mkfifo("PlayerToServer", 0666);
	
	fromPlayer = open("PlayerToServer", O_RDONLY, 0);
	
	remove("PlayerToServer");
	return fromPlayer;
	
}


int serverconnect(int fromPlayer){
	int playerindex;
	read(fromPlayer, &playerindex, sizeof(int));
	//printf("SERVER RECIEVED INDEX: %d \n", playerindex);
	int toPlayer;
	toPlayer = open("ServerToPlayer", O_WRONLY, 0);
	int playerindex2 = playerindex + 1;
	write(toPlayer, &playerindex2, sizeof(int));
	return toPlayer;
	

}


static void ignore_SIGPIPE(int signum){

	

}

