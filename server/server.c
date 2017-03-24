#include "server.h"

Player people_list[100];
int sockfd;
int size = 0;
char* IP = "127.0.0.1";
short PORT = 10222;
int flag, proflag;
typedef struct sockaddr SA;


void init_server(Card* poker)
{
    int optval = 1;
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
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
        perror("setsockopt fail");
        close(sockfd);
        exit(-1);
    }
    if(bind(sockfd, (SA*)&addr, sizeof(addr)) == -1){
        perror("bind failed\n");
        printf("Game server start failed...(bind)\n");
        exit(-1);
    }
    printf("Bind success !\n");
    if(listen(sockfd,100) == -1){
        perror("listen failed");
        printf("game server start failed");
        exit(-1);
    }
    printf("Initialize the game server successfully\n");
    init_poker(poker);
}

void send_msg_to_all(char *msg)
{
    int i = 0;
    for(; i <= size; i++){
        if(people_list[i].fds != 0){
            send(people_list[i].fds,msg,strlen(msg),0);
        }
    }
}

void *service_thread(void *p)
{
    printf("the service _thread\n");
    int m_id = 0;
    char name[20] = {};
    if(recv(people_list[size].fds, name, sizeof(name), 0) > 0){
        printf("the name is %s\n", name);
        strcpy(people_list[size].name, name);
    }
    m_id = size;
    size++;
    char tishi[100] = {};
    sprintf(tishi, "%s is the homeowner, Welcome %s join the game !", people_list[0].name,name);
    send_msg_to_all(tishi);
    int fd = ((struct pthst*)p)->plist->fds;
    char buf[100] = {};
    char msg[100] = {};
    int i =0;
     do{
        people_list[m_id].iscard = 1;
        while(strcmp(people_list[m_id].name,people_list[0].name) == 0){
            if((recv(fd, buf, sizeof(buf), 0 ) > 0) && (strncmp(buf, "start", 5) == 0)){
                send_msg_to_all(buf);
                shuffle_poker(((struct pthst *)p)->poker);
                deal2(((struct pthst *)p)->poker, &people_list[0], &people_list[1]);
                for(i = 0; i < size; i++){
                    sort(&people_list[i].handcards);
                    people_list[i].type = judge_type(people_list[i].handcards);
                    if(people_list[i].type == SCATTERED)
                        sort_scattered(people_list[i].handcards);
                }
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            while(1){
                if(flag == 1){
                    flag = 0;
                    break;
                }
            }
        }
        bzero(msg, 100);
        printf("\n%s:\n%s CARDS:\n\t %s  %s\t %s  %s\t %s  %s\n",people_list[m_id].name, cardtype[people_list[m_id].type-1],   \
                 suitname[people_list[m_id].handcards[0].suit],pointname[people_list[m_id].handcards[0].point],suitname[people_list[m_id].handcards[1].suit],  \
                 pointname[people_list[m_id].handcards[1].point],suitname[people_list[m_id].handcards[2].suit],pointname[people_list[m_id].handcards[2].point]);
        printf("abc\n");
        sprintf(msg, "\n%s:\n%s CARDS:\n\t %s  %s\t %s  %s\t %s  %s",people_list[m_id].name, cardtype[people_list[m_id].type-1],   \
                 suitname[people_list[m_id].handcards[0].suit],pointname[people_list[m_id].handcards[0].point],suitname[people_list[m_id].handcards[1].suit],  \
                 pointname[people_list[m_id].handcards[1].point],suitname[people_list[m_id].handcards[2].suit],pointname[people_list[m_id].handcards[2].point]);
        int num = send(people_list[m_id].fds, msg, strlen(msg), 0);
        printf("num = %d\n", num);
        if(recv(fd, buf, sizeof(buf), 0 ) <= 0){
            int i;
            for(i = 0; i < size; i++){
                if(people_list[i].fds == fd){
                    people_list[i].fds = 0;
                    break;
                }
            }
            printf(msg, "%s quit the game...", people_list[i].name);
            send_msg_to_all(msg);
            return;
        }
        if(strncmp(buf[0],"0", 1) == 0){
            bzero(buf,100);
            bzero(msg, 200);
            people_list[m_id].iscard == 0;
            int win;
            for(i = 0; i < size; i++){
                if(people_list[i].iscard == 1)
                    win = i;
            }
            printf(msg, "\n%s give up the game, %s  win !!!", people_list[m_id].name,people_list[m_id].name);
            send_msg_to_all(msg);
            bzero(msg,200);
            printf(msg,"\nYou lost \n");
            send(people_list[m_id].fds, msg, strlen(msg), 0);
            }
    }while((strncmp(buf, "Y", 1) == 0 )||(strncmp(buf, "y", 1) == 0));
    return;
}

void sig_close(){
    close(sockfd);
    printf("Game server closed..\n");
    exit(0);
}
void service(Card *poker){
    printf("The game server start service\n");
    int i = 0;
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
        struct pthst pthp;
        pthp.poker = poker; 
        pthp.plist = people_list;
        pthread_t pid;
        pthread_create(&pid , 0, service_thread, &pthp);
    }
}





















