libgame.so:game.o
	$(CC) -shared game.o -o libgame.so
game.o:game.c
	$(CC) -c -fpic game.c
clean:
	rm -rf *.o *.so
