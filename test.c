#include "server.h"


int main(){


	int* tes = malloc(sizeof(int) * 24);
	for(int i = 0; i < 24; i ++){
		tes[i] = generateindex();

	}
	printf("OK \n");
	tes = insertionsort(tes, 24);
	printf("OK \n");
	for(int j = 0; j < 24; j ++){
		printf("%d ", tes[j]);

	}
}

int* insertionsort(int* listofindices, int size){
	
	for(int i = 1; i < size; i ++){
		
		for(int j = i; j >= 1; j --){
			printf("J: %d \n" , j);
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
