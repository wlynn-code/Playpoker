CC = gcc
ROOT_DIR = $(shell pwd)
LD_LIBRARY_PATH = $(ROOT_DIR)/game
all:
	make -C $(ROOT_DIR)/game
	make -C $(ROOT_DIR)/server
	make -C $(ROOT_DIR)/client
clean:
	make clean -C $(ROOT_DIR)/game
	make clean -C $(ROOT_DIR)/server
	make clean -C $(ROOT_DIR)/client
