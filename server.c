#include "server.h"
#include "game.h"

void init()
{
	printf("Game server start...\n");
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("Create socket failed .");
		printf("Game server start failed...(socket)\n");
		exit(-1);
	}
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	if(bind(sockfd, (SA*)&addr, sizeof(addr)) == -1){
		perror("bind failed\n");
		printf("Game server start failed...(bind)\n");
		exit(-1);
	}
	printf("Bind success !\n");
	printf("Initialize the game server successfully");
}

void send_msg_to_all(char *msg)
{
	int i = 0;
	for(; i < size; i++){
		if(people_list[i].fds != 0){
			send(people_list[i].fds,msg,strlen(msg),0);
		}
	}
}

void *service_thread(void *p)
{
	int m_id = 0;
	char name[20] = {};
	if(recv(people_list[size].fds, name, sizeof(name), 0) > 0){
		strcpy(people_list[size].name, name);
	}
	m_id = size;
	size++;
	char tishi[100] = {};
	sprintf(tishi, "Welcome %s join the game\n");
	send_msg_to_all(tishi);
	int fd = *(int)p;
	while(1){
		char buf[100] = {};
	}
}
void service(){
	printf("The game server start service\n");
	while(1){
		struct sockaddr_in fromaddr;
		socklen_t len = sizeof(fromaddr);
		int fd = accept(sockfd,(SA*)&fromaddr,&len);
		if(fd == -1){
			printf("The client connect error..\n");
			continue;
		}
		people_list[size].fds = fd;
		printf("fd = %d\n", fd);
		pthread_t pid;
		pthread_create(&pid , 0, service_thread, &people_list[size].fds);
	}
}





















