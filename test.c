#include "server.h"


int main(){


	//int* tes = malloc(sizeof(int) * 24);
	//for(int i = 0; i < 24; i ++){
		//tes[i] = generateindex();

	//}
	//printf("OK \n");
	//tes = insertionsort(tes, 24);
	//printf("OK \n");
	//for(int j = 0; j < 24; j ++){
		//printf("%d ", tes[j]);

	//}


	struct connection** listofconnections2 = malloc(sizeof(struct connection) * 6);
	for(int i = 0; i < 4; i ++){

		struct connection* con = malloc(sizeof(struct connection));
		con -> fromPlayer1 = i;
		con -> toPlayer1 = i;
		con -> fromPlayer2 = i;
		con -> toPlayer2 = i;
		listofconnections2[i] = con;	

	}
	printconnections(listofconnections2);
	listofconnections2 = sortconnections(listofconnections2);
	printconnections(listofconnections2);
	

	printf("------------------------------- \n");
	
	
	listofconnections2[3] -> toPlayer1 = 30;
	listofconnections2[3] -> fromPlayer1 = 30;
	listofconnections2[2] -> toPlayer2 = 20;
	listofconnections2[2] -> fromPlayer2 = 20;
	

	listofconnections2[3] -> toWinner = 30;
	listofconnections2[3] -> fromWinner = 30;
	listofconnections2[2] -> toWinner = 20;
	listofconnections2[2] -> fromWinner = 20;
	printconnections(listofconnections2);
	printf("------------------------------- \n");
	remakeconnections(listofconnections2);
	printconnections(listofconnections2);

	
	printf("------------------------------- \n");


	


}


int remakeconnections(struct connection** listofconnections2){
	
	
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
					}
					else{
						con -> toPlayer1 = con2 -> toWinner;
						con -> fromPlayer1 = con2 -> fromWinner;
						
						con -> toWinner = -1;
						con -> fromWinner = -1;
						if(con2 -> fromPlayer1 == con2 -> fromWinner){
							printf("WORKS \n");
							con -> indexPlayer1 = con2 -> indexPlayer1;
							con2 -> indexPlayer1 = -1;
							con2 -> indexPlayer2 = -1;

						}
						else{
							printf("DOESNT \n");
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
					}
					
				}
				

			}
			con -> toWinner = -1;
			con -> fromWinner = -1;
			

		}


	}



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
