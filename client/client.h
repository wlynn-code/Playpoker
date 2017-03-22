#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "../game/game.h"

extern int init_game(char *namebuf);
extern void play_with_computer(char *name);
extern void play_with_outers(char *name);
