#include "player.h"




int main(){
	int toPlayer;
	int fromPlayer;
	toPlayer = playerhandshake(&fromPlayer);
	while(1){
		//read until msg says "pass"
		if()


	

	}
}



int random_index(){
	int randomIndex;
	int randFile = open("/dev/random", O_RDONLY, 0);
	read(randFile, &randomIndex, sizeof(int));
	if(randomIndex < 0){
		randomIndex *= -1;

	}
	printf("Generated Player Index: %d \n", randomIndex);
	randomIndex %= 1001;
	return randomIndex;
	//always positive, between 0 and 1000

}


int playerhandshake(int* fromPlayer){
	int toPlayer;
	mkfifo("ServerToPlayer", 0666);
	*fromPlayer = open("PlayerToServer", O_WRONLY, 0);
	int playerindex = random_index();
	//send the server playerindex for sorting
	write(*fromPlayer, &playerindex, sizeof(int));
	//print statements for debugging, will delete after
	//printf("PLAYER SENT INDEX: %d \n", playerindex);
	toPlayer = open("ServerToPlayer", O_RDONLY, 0);
	//wait for server to open ServerToPlayer
	remove("ServerToPlayer");
	int playerindex2;
	read(toPlayer, &playerindex2, sizeof(int));
	if(playerindex2 == playerindex + 1){
		//printf("PLAYER RECIEVED INDEX2: %d \n", playerindex2);
		int playerindex3 = playerindex2 + 1;
		write(*fromPlayer, &playerindex3, sizeof(int));
		//printf("PLAYER SENT INDEX3: %d \n", playerindex3);

	}
	//read SYN_ACK
	//send ACK(playerindex + 2)
	return toPlayer;
}
