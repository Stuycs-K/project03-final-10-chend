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
	

	


	struct connection** listofconnections2 = malloc(sizeof(int) * 24);
	for(int i = 0; i < 6; i ++){
			struct connection* con;
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
			
			
			//send this msg to any new players
			//started game
			//printf("Max number of players in tournament! \n");

			//max file desc
			int maxfile = -1;

			//zero out 
			FD_ZERO(&listofconnections);
			//select file desc again
			for(int i = 0; i < 6; i ++){
				struct connection* con = listofconnections2[i];
				if(con -> fromPlayer1 != -1){
					FD_SET(con -> fromPlayer1, &listofconnections);
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
			
			for(int i = 0; i < 6; i ++){
				struct connection* con = listofconnections2[i];
				if(FD_ISSET(con -> fromPlayer1, &listofconnections)){
					//SEND TO OTHER PLAYER
					//READ THE MSG
					int rockpaperscissors;
					read(con -> fromPlayer1, &rockpaperscissors, sizeof(int));
					write(con -> toPlayer2, rockpaperscissors, sizeof(int));
					//SECOND PLAYER WILL DECIDE WHO WINS, SENDS SERVER RESULT
					
				}
				else if(FD_ISSET(con -> fromPlayer2, &listofconnections)){
					int rockpaperscissors;
					read(con -> fromPlayer2, &rockpaperscissors, sizeof(int));
					write(con -> toPlayer1, rockpaperscissors, sizeof(int));
				}

			}


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

