#include "player.h"




int main(){
	int toPlayer;
	int fromPlayer;
	toPlayer = playerhandshake(&fromPlayer);

	//before quitting, send a message to the server by calling quit()
	signal(SIGINT, quit);
	returnfromPlayer(fromPlayer);
	int waiting = 0;
	printf("Waiting for server... \n");

	while(1){ 
		//wait for server to send "ready" once 8 players have joined
		//send "online" to tell server to unblock

		struct message* msg = malloc(sizeof(struct message));
		int bytes = read(toPlayer, msg, sizeof(struct message));
		if(bytes <= 0){
			printf("Server exited \n");
			exit(0);
		}
		if(strcmp(msg -> servermsg, "less") == 0){
			
			//less than 8 players
			//send msg(to unblock select)
			struct message* msg2 = malloc(sizeof(struct message));
			
			strcpy(msg2 -> servermsg, "online");
			
			write(fromPlayer, msg2, sizeof(struct message));
			sleep(0.1);

		}
		if(strcmp(msg -> servermsg, "ready") == 0){
			//break

			
			printf("Game Starting... \n");
			break;

		}

	}



	while(1){
		//game starts
		
		struct message* msg = malloc(sizeof(struct message));
		int bytes = read(toPlayer, msg, sizeof(struct message));
		//if no bytes were read, exit
		if(bytes <= 0){

			printf("Server Disconnected! \n");
			exit(0);
		}
		

		
		if(strcmp(msg -> servermsg, "recieveindex") == 0){
			//for player2
			//player1 got go
			int myindex = msg -> setindex;
			returnindex(myindex);
			int opponentindex = msg -> setindexopponent;
			printf("Your index is %d \n", myindex);
			printf("You are playing against index %d \n", opponentindex);


		}
		if(strcmp(msg -> servermsg, "pass") == 0){
			
			printf("Waiting for opponent... \n");




			
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
				
			}
		
			//WHO WON HERE
			if(msg -> value == 1000 || (choice == 1 && msg -> value == 3) || (choice == 2 && msg -> value == 1) || (choice == 3 && msg -> value == 2)){
				//win
				
				strcpy(newmsg -> servermsg, "won");
					
			}

			else if((choice == 3 && msg -> value == 1) || (choice == 1 && msg -> value == 2) || (choice == 2 && msg -> value == 3)){
				//lose
				

				strcpy(newmsg -> servermsg, "lose");
			}
			else{
				strcpy(newmsg -> servermsg, "draw");
				//tie(send go1)
				
			}
				
  			
 
			newmsg -> value = choice;
			if(msg -> value == 1000){
				newmsg -> value = 1000;

			}
			write(fromPlayer, newmsg, sizeof(struct message));
			waiting = 0;
		}
		//this player is first, asks for user input first and sends server msg
		else if(strcmp(msg -> servermsg, "go1") == 0){

			if(msg -> setindex >= 0 && msg -> setindexopponent >= 0){
				int myindex = msg -> setindex;
				int opponentindex = msg -> setindexopponent;
				printf("Your index is %d \n", myindex);
				printf("You are playing against index %d \n", opponentindex);
				

			}



			struct message* newmsg = malloc(sizeof(struct message));
			int choice = -1;
			while(choice != 1 && choice != 2 && choice != 3){
				printf("Please enter 1(rock), 2(paper), or 3(scissors)! \n");
				char* linebuff = malloc(256);
				fgets(linebuff, 255, stdin);
				sscanf(linebuff, "%d", &choice);		
				

			}
			
			newmsg -> value = choice;
			//MIGHT HAPPEN HERE
			
			strcpy(newmsg -> servermsg, "go2");
			write(fromPlayer, newmsg, sizeof(struct message));
			waiting = 0;

		}
		else if(strcmp(msg -> servermsg, "wonall") == 0){
			int choice = msg -> value;
			char* strchoice = malloc(256);
			if(choice == 1){
				strcpy(strchoice, "rock");
			}
			else if(choice == 2){

 				strcpy(strchoice, "paper");
			}
			else{

				strcpy(strchoice, "scissors");
			}
			printf("Player Won The Tournament! \n");
			if(choice == 1000){

				printf("Opponent disconnected \n");
			}
			
			else{
			printf("Opponent chose %s \n", strchoice);
			}
			quit(1);
		}
		else if(strcmp(msg -> servermsg, "won") == 0){
			int choice = msg -> value;
			char* strchoice = malloc(256);
			
			if(choice == 1){
				strcpy(strchoice, "rock");
			}
			else if(choice == 2){

 				strcpy(strchoice, "paper");
			}
			else{

				strcpy(strchoice, "scissors");
			}
		
			printf("Player Won! \n");
			if(choice == 1000){

				printf("Opponent disconnected \n");
			}
			else{
				printf("Opponent chose %s \n", strchoice);
			}
			printf("Waiting for new opponent... \n");
			waiting = 1;

		}
		else if(strcmp(msg -> servermsg, "lose") == 0){
			int choice = msg -> value;
			char* strchoice = malloc(256);
			if(choice == 1){
				strcpy(strchoice, "rock");
			}
			else if(choice == 2){

 				strcpy(strchoice, "paper");
			}
			else{

				strcpy(strchoice, "scissors");
			}
			printf("Player Lost! \n");
			printf("Opponent chose %s \n", strchoice);
			quit(1);

		}
		else if(strcmp(msg -> servermsg, "draw") == 0){
			int choice = msg -> value;
			char* strchoice = malloc(256);
			if(choice == 1){
				strcpy(strchoice, "rock");
			}
			else if(choice == 2){

 				strcpy(strchoice, "paper");
			}
			else{

				strcpy(strchoice, "scissors");
			}
			printf("Draw! \n");
			printf("Both players chose %s \n", strchoice);
			printf("New round starting... \n");

		}
		free(msg);
		msg = NULL;
	

	}
}





static void quit(int signum){
	
	int index = returnindex(0);
	int fromPlayer = returnfromPlayer(0);
	printf("Player Exiting... \n");
	struct message* msg = malloc(sizeof(struct message));
	strcpy(msg -> servermsg, "QUIT");
	msg -> setindex = index;
	write(fromPlayer, msg, sizeof(struct message));	
	sleep(1);
	//send QUIT to server
	
	sleep(1.6);
	exit(0);

}

static int returnfromPlayer(int fromPlayer){
	//keep track of fromPlayer
	static int fromPlay = 0;
	fromPlay += fromPlayer;
	return fromPlay;
	

}

static int returnindex(int index){
	//keep track of index
	static int myindex = 0;
	myindex += index;
	return myindex;

}

int random_index(){
	//generate a random index
	int randomIndex;
	int randFile = open("/dev/random", O_RDONLY, 0);
	read(randFile, &randomIndex, sizeof(int));
	if(randomIndex < 0){
		randomIndex *= -1;

	}
	
	randomIndex %= 1001;
	return randomIndex;
	//always positive, between 0 and 1000

}


int playerhandshake(int* fromPlayer){
	//player side of handshake
	//create ServerToPlayer pipe 


	int toPlayer;
	mkfifo("ServerToPlayer", 0666);
	chmod("ServerToPlayer", 0666);
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
