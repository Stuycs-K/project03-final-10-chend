compile: server.o player.o


	mkdir -p /tmp/chendfinal/
	chmod 777 /tmp/chendfinal/
	
	@gcc -o SERVER server.o
	@gcc -o PLAYER player.o
	cp SERVER /tmp/chendfinal
	cp PLAYER /tmp/chendfinal
	


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
