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
    char buf[100] = {};
    char msg[100] = {};
    int i =0;
     do{
        people_list[m_id].iscard = 1;
        while(strcmp(people_list[m_id].name,people_list[0].name) == 0){
            if((recv(people_list[0].fds, buf, sizeof(buf), 0 ) > 0) && (strncmp(buf, "start", 5) == 0)){
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
        printf("\n%s:\n%s CARDS:\n\t %s  %s\t %s  %s\t %s  %s",people_list[m_id].name, cardtype[people_list[m_id].type-1],   \
                 suitname[people_list[m_id].handcards[0].suit],pointname[people_list[m_id].handcards[0].point],suitname[people_list[m_id].handcards[1].suit],  \
                 pointname[people_list[m_id].handcards[1].point],suitname[people_list[m_id].handcards[2].suit],pointname[people_list[m_id].handcards[2].point]);
        bzero(msg, 100);
        sprintf(msg, "\n%s:\n%s CARDS:\n\t %s  %s\t %s  %s\t %s  %s",people_list[m_id].name, cardtype[people_list[m_id].type-1],   \
                 suitname[people_list[m_id].handcards[0].suit],pointname[people_list[m_id].handcards[0].point],suitname[people_list[m_id].handcards[1].suit],  \
                 pointname[people_list[m_id].handcards[1].point],suitname[people_list[m_id].handcards[2].suit],pointname[people_list[m_id].handcards[2].point]);
        int num = send(people_list[m_id].fds, msg, strlen(msg), 0);
        printf("num = %d\n", num);
        if((num = recv(people_list[m_id].fds, buf, sizeof(buf), 0) ) <= 0){
            int i;
            people_list[m_id].fds = 0;
            break;
            sprintf(msg, "%s quit the game...", people_list[i].name);
            send_msg_to_all(msg);
            return;
        }
        int win;
        if(strncmp(buf, "1", 1) == 0){
            people_list[m_id].iscard = 1;
        }
        if(strncmp(buf,"0", 1) == 0){
            bzero(buf,100);
            bzero(msg, 200);
            people_list[m_id].iscard = 0;
            for(i = 0; i < size; i++){
                if(people_list[i].iscard == 1)
                    win = i;
            }
            sprintf(msg, "\n%s give up the game\n", people_list[m_id].name);
            send_msg_to_all(msg);
        }
        printf("123\n");
        if((people_list[0].iscard == 1) && (people_list[1].iscard == 1)){
            if(compare2(&people_list[0],& people_list[1]) > 0)
                win = 0;
            else 
                win = 1;
        }
        printf("win = %d\n", win);
        printf("name = %s\n", people_list[m_id].name);
         if(strcmp(people_list[m_id].name, people_list[win].name) == 0){
            printf("****");
            bzero(buf, 100);
            sprintf(buf, "\nYou win !!!\n");
            send(people_list[m_id].fds, buf, sizeof(buf), 0);
        }
         if(strcmp(people_list[m_id].name, people_list[win].name) != 0){
            bzero(buf, 100);
            sprintf(buf,"\nYou lost  !!!\n");
            send(people_list[m_id].fds, buf, sizeof(buf), 0);
            bzero(msg, 200);
            sprintf(msg, "\n%s:\n%s CARDS:\n\t %s  %s\t %s  %s\t %s  %s",people_list[win].name, cardtype[people_list[win].type-1],   \
                 suitname[people_list[win].handcards[0].suit],pointname[people_list[win].handcards[0].point],suitname[people_list[win].handcards[1].suit],  \
                 pointname[people_list[win].handcards[1].point],suitname[people_list[win].handcards[2].suit],pointname[people_list[win].handcards[2].point]);
            send(people_list[m_id].fds, msg, sizeof(msg), 0); 
        }
         bzero(buf,100);
         recv(people_list[m_id].fds, buf, sizeof(buf), 0) ;
    }while((strncmp(buf, "Y", 1) == 0 )||(strncmp(buf, "y", 1) == 0));
    return;
}

void sig_close(){
    close(sockfd);
    printf("Game server closed..\n");
    exit(0);
}
void* service(void *p)
{
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
        pthp.poker = (Card *)p; 
        pthp.plist = people_list;
        pthread_t pid;
        pthread_create(&pid , 0, service_thread, &pthp);
    }
}
void start(Card *poker){
    printf("The game server start service\n");
    int i = 0;
    pthread_t pid;
    pthread_create(&pid, 0, service, &poker); 
    while(1){

    }
}





















