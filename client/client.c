#include "client.h"

int sockfd;
char *IP="127.0.0.1";
short PORT=10222;
int flag;
typedef struct sockaddr SA;
char name[30];

int init_game(char *namebuf)
{
    int people_flag=0;
    char people_name[20];
    printf("\nFirst step:\n\tPlese input  play with computer or others: \n");
    printf("\nHelp:\n");
    printf("\tinput 1: play with computer \n");
    printf("\tinput 2: play with others\n");
    scanf("%d", &people_flag);
    printf("Second step:\n\tPlese input your name:\n");
    scanf("%s", people_name);
    strcpy(namebuf, people_name);
    return people_flag;
}
void play_with_computer(char *name)
{
    Card poker[52];
    Player *winner = NULL;
    Player computer = {"computer"};
    Player people;
    strcpy(people.name, name);
    int base, money;
    printf("Third step:\n\t:Plese set money of computer:\n");
    scanf("%d", &computer.balance);
    printf("Forth step:\n\t:Plese set money of you:\n");
    scanf("%d", &people.balance);
    printf("Finally step:\n\t:Plese set the base money\n");
    scanf("%d", &base);
    printf("Loading...\n[");
    int i = 0;
    for(i = 0; i < 50; i++){
        printf(" ");
    } 
    printf(" ]\r[");
    for(i = 0; i < 51; i++){
        usleep(30000);
        printf("=");
        fflush(stdout);
    }
    sleep(1);
    printf("\n");
    printf("\n\tGREAT !!! Starting with computer !\n");
    char c;
    init_poker(&poker);
    do{
        printf("Handing...\n");
        sleep(1);
        shuffle_poker(poker);
        give_money(&people, base); 
        give_money(&computer, base);
        deal2(poker, &people, &computer);
        sort(&computer.handcards);
        sort(&people.handcards);
        computer.type = judge_type(&computer.handcards);
        if(computer.type == SCATTERED)
            sort_scattered(&computer.handcards);
        people.type = judge_type(&people.handcards);
        if(people.type == SCATTERED)
            sort_scattered(&people.handcards);
        show_playercards(&people);
        sleep(1);
        do{
            printf("Plesase input raise number or input 0 to give up\n");
            scanf("%d", &money);
            if(money < base && money != 0)
                printf("\nIput error,plese retry !");
        }while(money < base && money != 0);
        if (money != 0){
        give_money(&people, money);
        give_money(&computer, money);
        if(compare2(&people, &computer) > 0)
               winner = &people;
        else
        winner = &computer;
        }
        show_playercards(&computer);
        printf("The winner is:\n\t %s\n\n", winner->name);
        get_money(winner,(money+base)*2);
        printf("Your money:    %d\tComputer's money:    %d\n\n",  people.balance, computer.balance);
        if((computer.balance <= 0) ||( people.balance <= 0)){
            if(computer.balance <= 0){
                 printf("You win !!!\n");
                break;
            }
            else{
                printf("You lost  !!!\n");
            break;
            }
        }
        printf("Continue?  [Y/N]", people.balance, computer.balance);
        scanf(" %c", &c);
        printf("\n\n");
    }while(c=='Y'||c=='y');
    sleep(1);
    printf("\n\tGAME OVER!  THANKS\n");
    sleep(1);
    printf("\tDesigned by wlynn .\n");
    sleep(1);
    printf("\tThe game is for entertainment only, away from gambling !\n\n");
    sleep(2);
}

void init_client()
{
   printf("Game client start...\n");
    sockfd=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if(connect(sockfd,(SA*)&addr,sizeof(addr))==-1){
        perror("Can't connect to server\n");
        printf("Game client start failed\n");
        exit(-1);
    }
    printf("Game client start success\n");
}

void *recv_thread(void *p)
{
    while(1){
        char buf[200] = {};
        if(recv(sockfd, buf, sizeof(buf), 0) <= 0){
            return;
        }
        printf("%s\n", buf);
        if(strncmp(buf, "start", 5) ==0 )
            flag = 1;
    }
}

void start(char *name)
{
    int base, money;
    //printf("Finally step:\n\t:Plese set the base money\n");
    //scanf("%d", &base);
    printf("\nGame start !!!\n");
    char owner[100] = {};
    if(recv(sockfd, owner, sizeof(owner), 0) <= 0){
           return;
    }
    char buf[100];
    printf("%s\n", owner);
    pthread_t pid;
    pthread_create(&pid, 0, recv_thread, 0);
    char c;
    do{
        if(strncmp(owner, name, 3) == 0){
            printf("You are homeowner,please input start to deal cards !\n");
            scanf("%s", buf);
            while(strncmp(buf, "start", 5) != 0){
                printf("Input error, please retry !\n");
                bzero(buf,100);
                scanf("%s", buf);
            }
            int num = send(sockfd, buf, strlen(buf), 0);
            printf("sent start num %d\n", num);
        }
        if(flag ==  0){
            printf("Wating homeowner to input start...\n");
            while(1){
                if(flag == 1){
                    flag = 0;
                    break;
                }
            }
        }
        do{
            printf("Plesase input raise number or input 0 to give up\n");
            scanf("%d", &money);
            if(money < base && money != 0)
                printf("\nIput error,plese retry !");
        }while(money < base && money != 0);
        send(sockfd, money, sizeof(money), 0);
        sleep(2);
        printf("Continue?  [Y/N]");
        scanf(" %c", &c);
        printf("\n\n");
        send(sockfd, c, sizeof(c), 0);
    }while(c=='Y'||c=='y');
    sleep(1);
    printf("\n\tGAME OVER!  THANKS\n");
    sleep(1);
    printf("\tDesigned by wlynn .\n");
    sleep(1);
    printf("\tThe game is for entertainment only, away from gambling !\n\n");
    sleep(2);
}

void sig_close()
{
    close(sockfd);
    exit(0);
}

void play_with_others(char *name)
{
    signal(SIGINT, sig_close);
    init_client();
    int num = send(sockfd, name, strlen(name), 0);
    printf("send:%d\n", num);
    start(name);
}

