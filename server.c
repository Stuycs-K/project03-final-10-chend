#include server.h



int main(){


}


int** sort(int* unsorted){
	//[[], [], []] format, each subarray will contain 2 indices


}


int mainserversetup(){
	int fromPlayer;
	mkfifo("PlayerToServer", 0666);
	fromPlayer = open("PlayerToServer", O_RDONLY, 0);
	remove("PlayerToServer");
	return fromPlayer;
	
}


int serverconnect(int fromPlayer){
	int playerindex;
	read(fromPlayer, &playerindex, sizeof(int));
	printf("SERVER RECIEVED INDEX: %d \n", playerindex);
	int toPlayer;
	toPlayer = open("ServerToPlayer", O_WRONLY, 0);
	int playerindex2 = playerindex + 1;
	write(toPlayer, playerindex2, sizeof(int));
	
	

}


