objects = game.o start.o
start:$(objects)
	cc -o start $(objects)
$(objects):game.h
clean:
	rm start $(objects)

