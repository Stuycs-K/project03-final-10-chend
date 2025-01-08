compile: server.o player.o
	@gcc -o SERVER server.o
	@gcc -o PLAYER player.o




server.o: server.c server.h
	@gcc -c server.c


player.o: player.c player.h
	@gcc -c player.c


main: SERVER
	@./SERVER


play: PLAYER
	@./PLAYER


clean:
	@rm *.o
	@rm SERVER
	@rm PLAYER
