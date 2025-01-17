#include "server.h"



int main(){
	
	//numplayers is the max
	int numplayers = 8;
	int curplayers = 0;
	int unsorted = 1;

	signal(SIGPIPE, ignore_SIGPIPE);
	
	
	

	
	int history = open("history.txt", O_RDWR | O_APPEND | O_TRUNC , 0666);
	
	struct connection** listofconnections2 = malloc(sizeof(struct connection) * 4);
	for(int i = 0; i < 4; i ++){
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
			
			for(int i = 0; i < 4; i ++){

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
			fd_set listofconnections;
			if(unsorted){
				
				listofconnections2 = sortconnections(listofconnections2);
				for(int i = 0; i < 4; i ++){
					struct connection* con = listofconnections2[i];

					struct message* proceed = malloc(sizeof(struct message));
					strcpy(proceed -> servermsg, "go1");
					proceed -> setindex = con -> indexPlayer1;
					proceed -> setindexopponent = con -> indexPlayer2;
					
					
					
					write(con -> toPlayer1, proceed, sizeof(struct message));
					
					struct message* proceed2 = malloc(sizeof(struct message));
					strcpy(proceed2 -> servermsg, "recieveindex");
					proceed2 -> setindex = con -> indexPlayer2;
					proceed2 -> setindexopponent = con -> indexPlayer1;


					write(con -> toPlayer2, proceed2, sizeof(struct message));
					sleep(1);
					//TO SEND ANOTHER MESSAGE

						

					struct message* proceed3 = malloc(sizeof(struct message));
					strcpy(proceed3 -> servermsg, "pass");


					write(con -> toPlayer2, proceed3, sizeof(struct message));
					
				}
				
				unsorted = 0;
			}
			

			//remake and check if resort did anything
	
			

			remakeconnections(listofconnections2);
			int numplayers = 0;
			int lastmatch = 0;
			//check if server should send "wonall" and end tournament
			for(int i = 0; i < 4; i++){
				struct connection* con = listofconnections2[i];
				if(con -> fromPlayer1 != -1){
					numplayers += 1;
				}
				if(con -> fromPlayer2 != -1){
					numplayers += 1;					
				}


			}
			if(numplayers == 2){
				lastmatch = 1;
			}

			

			//sort listofconnections2

			//send this msg to any new players
			//started game
			//printf("Max number of players in tournament! \n");

			//max file desc
			int maxfile = -1;
			FD_ZERO(&listofconnections);
			
			//select file desc again
			for(int i = 0; i < 4; i ++){
				struct connection* con = listofconnections2[i];
				if(con -> fromPlayer1 != -1 && con -> fromWinner == -1 && con -> toWinner == -1){
					
					//send msg to all player1s to init
					
					if(con -> fromPlayer1 > maxfile){
						maxfile = con-> fromPlayer1;

					}
					
					FD_SET(con -> fromPlayer1, &listofconnections);
				}
				if(con -> fromPlayer2 != -1 && con -> fromWinner == -1 && con -> toWinner == -1){
					
					if(con -> fromPlayer2 > maxfile){
						maxfile = con-> fromPlayer2;

					}
					FD_SET(con -> fromPlayer2, &listofconnections);
				}	
				

			}
			maxfile += 1;
			
			
			
			if(select(maxfile, &listofconnections, NULL, NULL, NULL) < 0){
				printf("SELECT ERROR! \n");

			}
			//once select returns, check each potential desc with FD_ISSET
			
			for(int i = 0; i < 4; i ++){
				struct connection* con = listofconnections2[i];
				struct message* msg = malloc(sizeof(struct message));
				
				if(FD_ISSET(con -> fromPlayer1, &listofconnections)){
					
					//SEND TO OTHER PLAYER
					//READ THE MSG
					struct message* newmsg = malloc(sizeof(struct message));

					//msg to other player
					struct message* newmsg2 = malloc(sizeof(struct message));
  
					int bytes = read(con -> fromPlayer1, msg, sizeof(struct message));
					if(bytes <= 0){
						//disconnected
						FD_CLR(con -> fromPlayer1, &listofconnections);
						printf("DISCONNECT \n");
						continue;
					}


					//write to history file

					if(msg -> value == 1 || msg -> value == 2 || msg -> value == 3){
					char* loghistory = malloc(256);
					int bytes2 = sprintf(loghistory, "Player %d chose: %d \n", con -> fromPlayer1, msg -> value);
					write(history, loghistory, bytes2);
					}
					


					
					returnplayer1choice(msg -> value);




					//handle disconnects during gametime
					if(strcmp(msg -> servermsg, "QUIT") == 0){
						
						//remember to write to file "player... disconnected"
						//send 1000 to player2
						//for player2 send server "lose"
						//newmsg for player2
						newmsg -> value = 1000;
						strcpy(newmsg -> servermsg, "won");
						write(con -> toPlayer2, newmsg, sizeof(struct message));
						//set toWinner and toPlayer here
						char* loghistory = malloc(256);
						int bytes = sprintf(loghistory, "Player %d disconnected", con -> fromPlayer1);
						con -> fromPlayer1 = -1;
						con -> toPlayer1 = -1;
						con -> toWinner = con -> toPlayer2;
						con -> fromWinner = con -> fromPlayer2;
					
						write(history, loghistory, bytes);

						sleep(1);
						
						continue;

					}
						
					if(strcmp(msg -> servermsg, "go2") == 0){
						
						strcpy(newmsg -> servermsg, "go2");
						newmsg -> value = msg -> value;
						//BUG MIGHT HAPPEN HERE
						

						write(con -> toPlayer2, newmsg, sizeof(struct message));
						
						strcpy(newmsg2 -> servermsg, "pass");
						 
						write(con -> toPlayer1, newmsg2, sizeof(struct message));
						
					}
					
					//SECOND PLAYER WILL DECIDE WHO WINS, SENDS SERVER RESULT
					//else for exit


					

					
				
				}
				else if(FD_ISSET(con -> fromPlayer2, &listofconnections)){
					int bytes = read(con -> fromPlayer2, msg, sizeof(struct message));
					if(bytes <= 0){
						FD_CLR(con -> fromPlayer2, &listofconnections);
						printf("DISCONNECT \n");
						continue;
					}
					struct message* newmsg = malloc(sizeof(struct message));
					//msg to other player
					struct message* newmsg2 = malloc(sizeof(struct message));
					if(strcmp(msg -> servermsg, "won") == 0){
						//SET WINNER HERE


						con -> toWinner = con -> toPlayer2;
						con -> fromWinner = con -> fromPlayer2;
						
						//win
						//tell player1 to exit(SIGINT)
						//send "lose" to player1 & the choice opponent made(send choice from player2 to player1, player2 already knows since they decided who won)
						//send "won" to player2
						int opponentchoice = returnplayer1choice(-1);
						
						if(lastmatch){
								strcpy(newmsg -> servermsg, "wonall");
								newmsg -> value = opponentchoice;



								char* loghistory = malloc(256);
								int bytes = sprintf(loghistory, "Player %d chose: %d \n", con -> fromPlayer2, msg -> value);
								write(history, loghistory, bytes);

								char* result = malloc(256);
								bytes = sprintf(result, "Player %d won the tournament!!! \n", con -> fromPlayer2);
								write(history, result, bytes);

	
						}
						else{
							strcpy(newmsg -> servermsg, "won");
							newmsg -> value = opponentchoice;



							char* loghistory = malloc(256);
							int bytes = sprintf(loghistory, "Player %d chose: %d \n", con -> fromPlayer2, msg -> value);
							write(history, loghistory, bytes);

							char* result = malloc(256);
							bytes = sprintf(result, "Player %d won \n", con -> fromPlayer2);
							write(history, result, bytes);



						}
						

						strcpy(newmsg2 -> servermsg, "lose");
						newmsg2 -> value = msg -> value;

						write(con -> toPlayer1, newmsg2, sizeof(struct message));
						printf("SERVER SENT PLAYER1: %d \n", newmsg2 -> value);
						write(con -> toPlayer2,	newmsg, sizeof(struct message));
						printf("SERVER SENT PLAYER2: %d \n", newmsg -> value);
						if(lastmatch){
							sleep(1);
							exit(1);

					}
						
					else if(strcmp(msg -> servermsg, "QUIT") == 0){
						
						//remember to write to file "player... disconnected"
						//send 1000 to player1
						//for player2 send server "lose"
						//newmsg for player2
						newmsg -> value = 1000;
						strcpy(newmsg -> servermsg, "won");
						write(con -> toPlayer1, newmsg, sizeof(struct message));
						//set toWinner and toPlayer here
						char* loghistory = malloc(256);
						int bytes = sprintf(loghistory, "Player %d disconnected", con -> fromPlayer2);
						con -> fromPlayer2 = -1;
						con -> toPlayer2 = -1;
						con -> toWinner = con -> toPlayer1;
						con -> fromWinner = con -> fromPlayer1;
						
						write(history, loghistory, bytes);

						sleep(1);
						
						continue;

					}
					}
					else if(strcmp(msg -> servermsg, "lose") == 0){
						con -> toWinner = con -> toPlayer1;
						con -> fromWinner = con -> fromPlayer1;

						//lose
						//tell player2 to exit(SIGINT)
						//send "won" to player1 & the choice oppoennt made
						//send "lose to player2
						int opponentchoice = returnplayer1choice(-1);

						if(lastmatch){
								strcpy(newmsg -> servermsg, "wonall");
								newmsg -> value = msg -> value;



								char* loghistory = malloc(256);
								int bytes = sprintf(loghistory, "Player %d chose: %d \n", con -> fromPlayer2, msg -> value);
								write(history, loghistory, bytes);

								char* result = malloc(256);
								bytes = sprintf(result, "Player %d won the tournament!!! \n", con -> fromPlayer1);
								write(history, result, bytes);

	
						}
						else{
							strcpy(newmsg -> servermsg, "won");
							newmsg -> value = msg -> value;



							char* loghistory = malloc(256);
							int bytes = sprintf(loghistory, "Player %d chose: %d \n", con -> fromPlayer2, msg -> value);
							write(history, loghistory, bytes);

							char* result = malloc(256);
							bytes = sprintf(result, "Player %d won \n", con -> fromPlayer1);
							write(history, result, bytes);



						}




						strcpy(newmsg2 -> servermsg, "lose");
						newmsg2 -> value = opponentchoice;

						write(con -> toPlayer1, newmsg, sizeof(struct message));
						write(con -> toPlayer2, newmsg2, sizeof(struct message));
						if(lastmatch){
							sleep(1);
							exit(1);

						}
						

					}
					else if(strcmp(msg -> servermsg, "draw") == 0){

						//draw
						

						strcpy(newmsg -> servermsg, "draw");
						newmsg -> value = msg -> value;

						strcpy(newmsg2 -> servermsg, "draw");
						newmsg2 -> value = msg -> value;
						write(con -> toPlayer1, newmsg, sizeof(struct message));
						write(con -> toPlayer2, newmsg2, sizeof(struct message));


						sleep(1);
						
						
						
						
						struct message* newmsg3 = malloc(sizeof(struct message));
						strcpy(newmsg3 -> servermsg, "go1");
						newmsg3 -> value = msg -> value;
						newmsg3 -> setindex = -1;
						newmsg3 -> setindexopponent = -1;
						//msg to other player
						struct message* newmsg4 = malloc(sizeof(struct message));
						newmsg4 -> value = msg -> value;
						strcpy(newmsg4 -> servermsg, "pass");
						write(con -> toPlayer1, newmsg3, sizeof(struct message));
						write(con -> toPlayer2, newmsg4, sizeof(struct message));
						char* loghistory = malloc(256);
						int bytes = sprintf(loghistory, "Player %d chose: %d \n", con -> fromPlayer2, msg -> value);
						write(history, loghistory, bytes);

						

						
					}
					
					//wonall if player wins final match




				}

			}
			//zero out 
			FD_ZERO(&listofconnections);
			sleep(0.5);
			//if so, get the message from that desc to the player its playing against
			//write to the opponent using 
		}
		
		


	}

}








static int returnplayer1choice(int player1choice){
	//int player1choice is either 1, 2, or 3(set value)
	
	static int Player1Choice = -1;
	if(player1choice != -1){
	Player1Choice = player1choice;
	printf("PLAY1 SET: %d \n", Player1Choice);
	}
	
	return Player1Choice;
	

}



void printconnections(struct connection** listofconnections2){
	for(int i = 0; i < 4; i ++){
		
		struct connection* con = listofconnections2[i];
		printf("PLAYER1INDEX: %d \n", con -> indexPlayer1);
		printf("FROMPLAYER1: %d \n", con -> fromPlayer1);
		printf("PLAYER2INDEX: %d \n", con -> indexPlayer2);
		printf("FROMPLAYER2: %d \n", con -> fromPlayer2);
		printf("TOWINNER: %d \n", con -> toWinner);
		printf("FROMWINNER: %d \n", con ->fromWinner);

	}


}



int remakeconnections(struct connection** listofconnections2){
	
	int resorted = 0;
	for(int i = 0; i < 4; i ++){
		struct connection* con = listofconnections2[i];
		//set winner to -1 on default when creating connections
		if(con -> toWinner != -1 && con -> fromWinner != -1){
			//check who won
			int play1 = 0;
			if(con -> toWinner == con -> toPlayer1 && con -> fromWinner == con -> fromPlayer1){
				play1 = 1;

			}





			for(int j = 0; j < 4; j ++){
				if(j == i){
					continue;
				}
				struct connection* con2 = listofconnections2[j];
				if(con2 -> toWinner != -1 && con2 -> fromWinner != -1){
					resorted = 1;
					if(play1){
						//add index as well
						con -> toPlayer2 = con2 -> toWinner;
						con -> fromPlayer2 = con2 -> fromWinner;
						
						con -> toWinner = -1;
						con -> fromWinner = -1;
						if(con2 -> fromPlayer1 == con2 -> fromWinner){
							con -> indexPlayer2 = con2 -> indexPlayer1;
							con2 -> indexPlayer1 = -1;
							con2 -> indexPlayer2 = -1;

						}
						else{
							con -> indexPlayer2 = con2 -> indexPlayer2;
							con2 -> indexPlayer1 = -1;
							con2 -> indexPlayer2 = -1;


						}
						
						//reset everything
						con2 -> toWinner = -1;
						con2 -> fromWinner = -1;
						con2 -> fromPlayer1 = -1;
						con2 -> toPlayer1  = -1;
						con2 -> fromPlayer2 = -1;
						con2 -> toPlayer2 = -1;
						
						//send go1
						struct message* msg = malloc(sizeof(struct message));
						strcpy(msg -> servermsg,"go1");
						msg -> setindex = con -> indexPlayer1;
						msg -> setindexopponent = con -> indexPlayer2;
						struct message* msg2 = malloc(sizeof(struct message));
						strcpy(msg2 -> servermsg, "recieveindex");
						msg2 -> setindex = con -> indexPlayer2;
						msg2 -> setindexopponent = con -> indexPlayer1;


						struct message* msg3 = malloc(sizeof(struct message));
						strcpy(msg3 -> servermsg, "pass");
						//server sends "recieve index", waits, then sends pass(go1 automatically prints index)
						write(con -> toPlayer1, msg, sizeof(struct message));
						write(con -> toPlayer2, msg2, sizeof(struct message));
						sleep(1);
						write(con -> toPlayer2, msg3, sizeof(struct message));


					}
					else{
						con -> toPlayer1 = con2 -> toWinner;
						con -> fromPlayer1 = con2 -> fromWinner;
						
						con -> toWinner = -1;
						con -> fromWinner = -1;
						if(con2 -> fromPlayer1 == con2 -> fromWinner){
							
							con -> indexPlayer1 = con2 -> indexPlayer1;
							con2 -> indexPlayer1 = -1;
							con2 -> indexPlayer2 = -1;

						}
						else{
							con -> indexPlayer1 = con2 -> indexPlayer2;
							con2 -> indexPlayer1 = -1;
							con2 -> indexPlayer2 = -1;


						}

						con2 -> toWinner = -1;
						con2 -> fromWinner = -1;
						con2 -> fromPlayer1 = -1;
						con2 -> toPlayer1  = -1;
						con2 -> fromPlayer2 = -1;
						con2 -> toPlayer2 = -1;
						//send pass
						struct message* msg = malloc(sizeof(struct message));
						strcpy(msg -> servermsg, "go1");
						msg -> setindex = con -> indexPlayer1;
						msg -> setindexopponent = con -> indexPlayer2;
						struct message* msg2 = malloc(sizeof(struct message));
						strcpy(msg2 -> servermsg, "recieveindex");
						msg2 -> setindex = con -> indexPlayer2;
						msg2 -> setindexopponent = con -> indexPlayer1;


						struct message* msg3 = malloc(sizeof(struct message));
						strcpy(msg3 -> servermsg,"pass");
						//server sends "recieve index", waits, then sends pass(go1 automatically prints index)
						write(con -> toPlayer1, msg, sizeof(struct message));
						write(con -> toPlayer2, msg2, sizeof(struct message));
						sleep(1);
						write(con -> toPlayer2, msg3, sizeof(struct message));
					}
					
				}
				

			}
			
		}


	}


	return resorted;
}



struct connection** sortconnections(struct connection** listofconnections2){
	int n = 0;
	struct connection** newlistofconnections2 = malloc(sizeof(struct connection) * 4);
	int* listofindices = malloc(sizeof(int) * 16);
	for(int i = 0; i < 4; i ++){
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
		for(int j = 0; j < 4; j ++){
			
			
			struct connection* con = listofconnections2[j];
			
			if(con -> indexPlayer1 == listofindices[i] || con -> indexPlayer1 == listofindices[i + 1]){
				if(newcon -> toPlayer1 == -1){
					newcon -> toPlayer1 = con -> toPlayer1;
					newcon -> fromPlayer1 = con -> fromPlayer1;
					newcon -> indexPlayer1 = con -> indexPlayer1;
					newcon -> toWinner = -1;
					newcon -> fromWinner = -1;
				}
				else if(newcon -> toPlayer2 == -1){
					newcon -> toPlayer2 = con -> toPlayer1;
					newcon -> fromPlayer2 = con -> fromPlayer1;
					newcon -> indexPlayer2 = con -> indexPlayer1;
					newcon -> toWinner = -1;
					newcon -> fromWinner = -1;
				}
				
				
				
				
			}
			
			if(con -> indexPlayer2 == listofindices[i] || con -> indexPlayer2 == listofindices[i + 1]){
				
				if(newcon -> toPlayer1 == -1){
					newcon -> toPlayer1 = con -> toPlayer2;
					newcon -> fromPlayer1 = con -> fromPlayer2;
					newcon -> indexPlayer1 = con -> indexPlayer2;	
					newcon -> toWinner = -1;
					newcon -> fromWinner = -1;	
				}
				else if(newcon -> toPlayer2 == -1){
					newcon -> toPlayer2 = con -> toPlayer2;
					newcon -> fromPlayer2 = con -> fromPlayer2;
					newcon -> indexPlayer2 = con -> indexPlayer2;
					newcon -> toWinner = -1;
					newcon -> fromWinner = -1;
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

