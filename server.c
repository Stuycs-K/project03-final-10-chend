#include "server.h"



int main(){
	int choosing = 0;
	int numplayers;
	int curplayers = 0;
	
	while(choosing){
		
		printf("Enter 2, 4, 8, or 12(number of players in the tournament): ");
		fgets(numplayers, sizeof(int), stdin);
		if(numplayers == 2 || numplayers == 4 || numplayers == 8 || numplayers == 12){
			
			printf("Waiting for %d players...\n", numplayers);
			choosing = 1;
		}
		else{
			printf("Please enter a valid number of players!");

		}
	}
	signal(SIGPIPE, ignore_SIGPIPE);
	fd_set listofconnections;
	FD_ZERO(&listofconnections);
	

	


	int** listofconnections2 = malloc(sizeof(int) * 12);
	for(int i = 0; i < 6; i ++){
			//array for file desc of 2 players
			int* connection = malloc(sizeof(int) * 2);
			connection[0] = -1;
			connection[1] = -1;
			listofconnections2[i] = connection;
	}
	while(1){
		if(curplayers < numplayers){
			int toPlayer;
			int fromPlayer;
			fromPlayer = mainserversetup();
			toPlayer = serverconnect(&fromPlayer);
			FD_SET(fromPlayer, &listofconnections);
			for(int i = 0; i < 6; i ++){

				if(listofconnections2[i][0] == -1){
					listofconnections2[i][0] = fromPlayer;
					break;
				}
				else if(listofconnections2[i][1] == -1){
					listofconnections2[i][1] = fromPlayer;
					break;
				}
			}
			curplayers ++;
			
		}
		else{
			
			
			//send this msg to any new players
			//started game
			//printf("Max number of players in tournament! \n");

			//zero out 
			FD_ZERO(&listofconnections);
			//select file desc again
			for(int i = 0; i < 6; i ++){
				if(listofconnections2[i][0] != -1){
					FD_SET(listofconnections2[i][0], &listofconnections);
					
				}
				if(listofconnections2[i][1] != -1){
					FD_SET(listofconnections2[i][1], &listofconnections);
					
				}

			}
			select( , &listofconnections, NULL, NULL, NULL);
			//once select returns, check each potential desc with FD_ISSET
			//if so, get the message from that desc to the player its playing against
			//write to the opponent using 
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

