#include "server.h"



int main(){
	
	//numplayers is the max
	int numplayers = 12;
	int curplayers = 0;
	int unsorted = 1;

	signal(SIGPIPE, ignore_SIGPIPE);
	fd_set listofconnections;
	FD_ZERO(&listofconnections);
	

	


	struct connection** listofconnections2 = malloc(sizeof(struct connection) * 6);
	for(int i = 0; i < 6; i ++){
			struct connection* con = malloc(sizeof(struct connection));
			con -> toPlayer1 = -1;
			con -> fromPlayer1 = -1;
			con -> toPlayer2 = -1;
			con -> fromPlayer2 = -1;
			listofconnections2[i] = con;
	}
	while(1){
		if(curplayers < numplayers){
			int toPlayer;
			int fromPlayer;
			printf("Waiting for %d players... \n", numplayers - curplayers);
			fromPlayer = mainserversetup();
			toPlayer = serverconnect(&fromPlayer);
			FD_SET(fromPlayer, &listofconnections);
			for(int i = 0; i < 6; i ++){

				struct connection* con = listofconnections2[i];
				if(con -> fromPlayer1 == -1){
					con -> fromPlayer1 = fromPlayer;
					con -> toPlayer1 = toPlayer;
					break;
				}
				else if(con -> fromPlayer2 == -1){
					con -> fromPlayer2 = fromPlayer;
					con -> toPlayer2 = toPlayer;
					break;

				}
				

			}
			curplayers ++;
			
		}
		else{
			if(unsorted){

				listofconnections2 = sortconnections(listofconnections2);
				unsorted = 0;
			}
			//sort listofconnections2

			//send this msg to any new players
			//started game
			//printf("Max number of players in tournament! \n");

			//max file desc
			int maxfile = -1;

			
			//select file desc again
			for(int i = 0; i < 6; i ++){
				struct connection* con = listofconnections2[i];
				if(con -> fromPlayer1 != -1){
					FD_SET(con -> fromPlayer1, &listofconnections);
					//send msg to all player1s to init
					struct message* proceed = malloc(sizeof(struct message));
					strcpy(proceed -> servermsg, "go1");
					write(con -> toPlayer1, proceed, sizeof(struct message));
					
				
					if(con -> fromPlayer1 > maxfile){
						maxfile = con-> fromPlayer1;

					}
				}
				if(con -> fromPlayer2 != -1){
					FD_SET(con -> fromPlayer2, &listofconnections);
					if(con -> fromPlayer2 > maxfile){
						maxfile = con-> fromPlayer2;

					}
				}	
				

			}

			
			
			
			select(maxfile + 1, &listofconnections, NULL, NULL, NULL);
			//once select returns, check each potential desc with FD_ISSET
			printf("RAN \n");
			for(int i = 0; i < 6; i ++){
				struct connection* con = listofconnections2[i];
				struct message* msg = malloc(sizeof(struct message));
				
				if(FD_ISSET(con -> fromPlayer1, &listofconnections)){
					
					//SEND TO OTHER PLAYER
					//READ THE MSG
					struct message* newmsg = malloc(sizeof(struct message));
					read(con -> fromPlayer1, msg, sizeof(struct message));
					printf("PLAYER1 VAL: %d \n", msg -> value);



					if(strcmp(msg -> servermsg, "go2") == 0){
						strcpy(newmsg -> servermsg, "go2");
						newmsg -> value = msg -> value;
						write(con -> toPlayer2, newmsg, sizeof(struct message));
						
						
					}
					
					//SECOND PLAYER WILL DECIDE WHO WINS, SENDS SERVER RESULT
					//else for exit
				}
				else if(FD_ISSET(con -> fromPlayer2, &listofconnections)){
					read(con -> fromPlayer2, msg, sizeof(struct message));
					struct message* newmsg = malloc(sizeof(struct message));

					if(strcmp(msg -> servermsg, "won") == 0){

						//win
						//tell player1 to exit(SIGINT)
						//send "pass" to player2
						
					}
					else if(strcmp(msg -> servermsg, "lose") == 0){

						//lose
						//tell player2 to exit(SIGINT)
						//send "pass" to player1
					}
					else{

						//draw
					}
					




				}

			}
			//zero out 
			FD_ZERO(&listofconnections);

			//if so, get the message from that desc to the player its playing against
			//write to the opponent using 
		}
		
		


	}

}




void printconnections(struct connection** listofconnections2){
	for(int i = 0; i < 6; i ++){
		
		struct connection* con = listofconnections2[i];
		printf("PLAYER1INDEX: %d \n", con -> indexPlayer1);
		printf("FROMPLAYER1: %d \n", con -> fromPlayer1);
		printf("PLAYER2INDEX: %d \n", con -> indexPlayer2);
		printf("FROMPLAYER2: %d \n", con -> fromPlayer2);

	}


}

struct connection** sortconnections(struct connection** listofconnections2){
	int n = 0;
	struct connection** newlistofconnections2 = malloc(sizeof(struct connection) * 6);
	int* listofindices = malloc(sizeof(int) * 24);
	for(int i = 0; i < 6; i ++){
		//check if not empty
		struct connection* con = listofconnections2[i];
		con -> indexPlayer1 = generateindex();
		
		//might need to sleep here
		con -> indexPlayer2 = generateindex();
		
		listofindices[n] = con -> indexPlayer1;
		n += 1;
		listofindices[n] = con -> indexPlayer2;
		n += 1;


	}
	listofindices = insertionsort(listofindices, n);
	
	int qq = 0;
	for(int i = 0; i < n; i += 2){
		struct connection* newcon = malloc(sizeof(struct connection));
		newcon -> toPlayer1 = -1;
		newcon -> toPlayer2 = -1;
		for(int j = 0; j < 6; j ++){
			
			
			struct connection* con = listofconnections2[j];
			
			if(con -> indexPlayer1 == listofindices[i] || con -> indexPlayer1 == listofindices[i + 1]){
				if(newcon -> toPlayer1 == -1){
					newcon -> toPlayer1 = con -> toPlayer1;
					newcon -> fromPlayer1 = con -> fromPlayer1;
					newcon -> indexPlayer1 = con -> indexPlayer1;
				}
				else if(newcon -> toPlayer2 == -1){
					newcon -> toPlayer2 = con -> toPlayer1;
					newcon -> fromPlayer2 = con -> fromPlayer1;
					newcon -> indexPlayer2 = con -> indexPlayer1;
				}
				
				
				
				
			}
			
			if(con -> indexPlayer2 == listofindices[i] || con -> indexPlayer2 == listofindices[i + 1]){
				
				if(newcon -> toPlayer1 == -1){
					newcon -> toPlayer1 = con -> toPlayer2;
					newcon -> fromPlayer1 = con -> fromPlayer2;
					newcon -> indexPlayer1 = con -> indexPlayer2;		
				}
				else if(newcon -> toPlayer2 == -1){
					newcon -> toPlayer2 = con -> toPlayer2;
					newcon -> fromPlayer2 = con -> fromPlayer2;
					newcon -> indexPlayer2 = con -> indexPlayer2;
				}
				
				

			}
			
		}
		newlistofconnections2[qq] = newcon;
		
		qq += 1;
			
		
	}

	return newlistofconnections2;

}


int* insertionsort(int* listofindices, int size){
	
	for(int i = 1; i < size; i ++){
		
		for(int j = i; j >= 1; j --){
			
			if(listofindices[j] <= listofindices[j - 1]){
				int tempval2 = listofindices[j - 1];
				int tempval = listofindices[j];
				listofindices[j] = tempval2;
				listofindices[j - 1] = tempval;

			}

		}
		

	}
	return listofindices;
	
}


int generateindex(){

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

