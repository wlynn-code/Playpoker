LD_LIBRARY_PATH = ../game
objects_client = client.o start_client.o
start_client:$(objects_client)
	cc -o client_start $(objects_client) -lpthread -lgame -L $(LD_LIBRARY_PATH)
$(objects_server):client.h
clean:
	rm -rf start_client  $(objects_client)

