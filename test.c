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
	for(int i = 0; i < 6; i ++){

		struct connection* con = malloc(sizeof(struct connection));
		con -> fromPlayer1 = i;
		con -> toPlayer1 = i;
		con -> fromPlayer2 = i;
		con -> fromPlayer2 = i;
		listofconnections2[i] = con;	

	}
	printconnections(listofconnections2);
	listofconnections2 = sortconnections(listofconnections2);
	printconnections(listofconnections2);


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
		printf("GENERATEDINDEX1: %d \n", con -> indexPlayer1);
		//might need to sleep here
		con -> indexPlayer2 = generateindex();
		printf("GENERATEDINDEX2: %d \n", con -> indexPlayer2);	
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
			printf("CON INDEX1: %d , CON INDEX2: %d \n", con ->indexPlayer1, con ->indexPlayer2);
			printf("CHECKING FOR1: %d, CHECKING FOR2: %d \n", listofindices[i], listofindices[i + 1]);
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
		printf("NEWCON INDEX1: %d \n", newcon ->indexPlayer1);
		printf("NEWCON INDEX2: %d \n", newcon ->indexPlayer2);
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
