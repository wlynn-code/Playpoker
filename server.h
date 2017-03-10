#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

Player people_list[100] = {0};
int sockfd;
int size = 0;
char* IP = "127.0.0.1";
short PORT = 10222;
typedef struct sockaddr SA;
