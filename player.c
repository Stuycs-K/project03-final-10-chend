#include "player.h"




int main(){
	int toPlayer;
	int fromPlayer;
	toPlayer = playerhandshake(&fromPlayer);

	while(1){
		//read until msg says "pass"
		struct message* msg = malloc(sizeof(struct message));
		read(toPlayer, msg, sizeof(struct message));
		if(strcmp(msg -> servermsg, "pass") == 0){
			
			printf("Waiting for players... \n");
		}
		if(strcmp(msg -> servermsg, "go2") == 0){
			//THIS PLAYER IS SECOND, CALCULATES WHO WON AND SENDS SERVER WINNER
			struct message* newmsg = malloc(sizeof(struct message));
			int choice = -1;
			while(choice != 1 && choice != 2 && choice != 3){
				printf("Please enter 1(rock), 2(paper), or 3(scissors)! \n");
				char* linebuff = malloc(256);
				
				fgets(linebuff, 255, stdin);
				sscanf(linebuff, "%d", &choice);				
				printf("CHOICE: %d \n", choice);
			}
			printf("CHOICE: %d \n", choice);
			//WHO WON HERE
			if((choice == 1 && msg -> value == 3) || (choice == 2 && msg -> value == 1) || (choice == 3 && msg -> value == 2)){
				//win
				strcpy(newmsg -> servermsg, "won");
					
			}

			else if((choice == 3 && msg -> value == 1) || (choice == 1 && msg -> value == 2) || (choice == 2 && msg -> value == 3)){
				//lose
				strcpy(newmsg -> servermsg, "lose");
			}
			else{
				strcpy(newmsg -> servermsg, "go1");
				//tie(send go1)
				
			}
				
  			
 
			newmsg -> value = choice;
			
			write(fromPlayer, newmsg, sizeof(struct message));

		}
		else if(strcmp(msg -> servermsg, "go1") == 0){
			struct message* newmsg = malloc(sizeof(struct message));
			int choice = -1;
			while(choice != 1 && choice != 2 && choice != 3){
				printf("Please enter 1(rock), 2(paper), or 3(scissors)! \n");
				char* linebuff = malloc(256);
				fgets(linebuff, 255, stdin);
				sscanf(linebuff, "%d", &choice);		
				

			}
			
			newmsg -> value = choice;
			strcpy(newmsg -> servermsg, "go2");
			write(fromPlayer, newmsg, sizeof(struct message));

		}

	

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
