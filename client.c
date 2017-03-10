#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

int sockfd;
char *IP="127.0.0.1";
short PORT=10222;
typedef struct sockaddr SA;
char name[30];//\u7528\u4e8e\u5b58\u653e\u804a\u5929\u4eba\u7684\u6635\u79f0

//\u5ba2\u6237\u7aef\u4e5f\u5206\u4e3a\u51e0\u6b65:
//\u542f\u52a8\u5ba2\u6237\u7aef\uff0c\u8fde\u63a5\u670d\u52a1\u5668
void init(){
	printf("\u804a\u5929\u5ba4\u5ba2\u6237\u7aef\u5f00\u59cb\u542f\u52a8\n");
	sockfd=socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	if(connect(sockfd,(SA*)&addr,sizeof(addr))==-1){
		perror("\u65e0\u6cd5\u8fde\u63a5\u5230\u670d\u52a1\u5668");
		printf("\u5ba2\u6237\u7aef\u542f\u52a8\u5931\u8d25\n");
		exit(-1);
	}
	printf("\u5ba2\u6237\u7aef\u542f\u52a8\u6210\u529f\n");
}
//\u5f00\u59cb\u901a\u4fe1
void start(){
	pthread_t pid;
	void* recv_thread(void*);
	pthread_create(&pid,0,recv_thread,0);
	char buf[100] = {0};
	while(1){
			send(sockfd,buf,strlen(buf),0);
	}
}
void * recv_thread(void* p){
	while(1){
		char buf[100]={};
		if(recv(sockfd,buf,sizeof(buf),0)<=0){
			return ;
		}
		printf("%s\n",buf);
	}
}
void sig_close(){
	close(sockfd);
	exit(0);
}
int main(){
	signal(SIGINT,sig_close);
	printf("\u8bf7\u8f93\u5165\u60a8\u7684\u540d\u5b57:");
	scanf("%s",name);
	//char buf[100];
	//sprintf(buf,"%s\u767b\u5f55\u4e86\u804a\u5929\u5ba4..",name);
	init();
	send(sockfd,name,strlen(name),0);
	start();
}

