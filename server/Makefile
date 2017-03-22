LD_LIBRARY_PATH = ../game
objects_server =  server.o start_server.o
start_server:$(objects_server)
	cc -o server_start $(objects_server) -lpthread -lgame -L$(LD_LIBRARY_PATH)
$(objects_server):server.h
clean:
	rm -rf start_server  $(objects_server)

