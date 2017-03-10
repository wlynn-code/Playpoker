#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

const char* cardtype[ ] =  _HANDCARDS_TYPE;
const char* suitname[ ] = {"HEARTS", "SPADES", "DIAMONDS", "CLUBS"};
const char* pointname[ ] = { " ",  "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};


void init_poker( Card *poker)
{
	int i = 0, suit = 0, point = 0;
	for(i = suit; suit < 4; suit++){
		for(point = 2; point <= 14; point++){
			poker[i].suit = suit;
			poker[i].point = point;
			i++;
		}
	}	
}

void swap(Card *p, Card *q)
{
	Card t = *p;
	*p = *q;
	*q = t;
}



int init_game(Card *poker, Player* plst_arr){
	int people_num=0, i =0;
	char people_name[20];
	printf("\nFirst step:\n\tPlese input how many people to play : \n");
	printf("\nHelp:\n");
	printf("\tinput 1: play with computer \n");
	printf("\tinput 2 or 3 or ... : play with others\n");
	scanf("%d", &people_num);
	for(i = 0; i < people_num; i++){ 
		printf("Second step:\n\tPlese input your name:\n");
		scanf("%s", people_name);
		strcpy(plst_arr[i].name, people_name);
	}
	return people_num;
}

void shuffle_poker(Card *poker)
{
	int i = 0;
	srand(time(NULL));
	for(i = 0; i  < 500; i++){
		swap( &poker[rand( )%52], &poker[rand( )%52]);
	}
}

void give_money(Player *p, int money)
{
	p->balance -= money;
}

void get_money(Player *p, int money)
{
	p->balance += money;
}

void deal2(Card *poker, Player *p, Player *q)
{
	int i = 0;
	for(i = 0; i < 3; i++){
		p->handcards[i] = poker[ 2*i];
		q->handcards[i] = poker[2*i+1]; 
	}
}

void sort(Card *handcards)
{
	if(handcards[0].point > handcards[1].point)
		swap(&handcards[0], &handcards[1]);
	if(handcards[0].point> handcards[2].point)
		swap(&handcards[0], &handcards[2]);
	if(handcards[1].point > handcards[2].point)
		swap(&handcards[1], &handcards[2]);
	if(handcards[1].point == handcards[2].point)      //PAIR
		swap(&handcards[0], &handcards[2]);
	if((handcards[0].point == 2)  && (handcards[1].point == 3) && (handcards[2].point == 14)) {   //pl.  23A -> A23
		swap(&handcards[1], &handcards[2]);
		swap(&handcards[0], &handcards[1]);
	}
	if(handcards[2].point == 14)
		swap(&handcards[0], &handcards[2]);
}

void sort_scattered(Card *handcards)
{
	if(handcards[0].point < handcards[1].point)
		swap(&handcards[0], &handcards[1]);
	if(handcards[0].point < handcards[2].point)
		swap(&handcards[0], &handcards[2]);
	if(handcards[1].point < handcards[2].point)
		swap(&handcards[1], &handcards[2]);
}

int  judge_type(Card *handcards)
{
	if((handcards[0].point == 2) && (handcards[1].point == 3) && (handcards[2].point == 5))
		return SPECIAL;
	else if(handcards[0].point == handcards[1].point){
		if(handcards[1].point == handcards[2].point)
			return SET;
		else
			return PAIR;
	}
	else if(((handcards[0].point+1) == (handcards[1].point) == (handcards[2].point -1)) ||((handcards[0].point == 14) && (handcards[1].point == 2) && (handcards[2].point == 3))){
		if(handcards[0].suit == handcards[1].suit == handcards[2].suit)
			return STRAIGHT_FLUSH;
		else 
			return STRAIGHT;
	}
	else if((handcards[0].suit == handcards[1].suit) && (handcards[1].suit == handcards[2].suit))
		return FLUSH;
	else
		return SCATTERED;
}

void show_playercards(Player *p)
{	
	int i = 0;
	printf("\n%s\n:", p->name);	
	printf("%s CARDS:\n", cardtype[p->type-1]);
	printf("\b");
	for(i = 0; i < 3; i++){
		printf("\t %s  %s", suitname[p->handcards[i].suit], pointname[p->handcards[i].point]);
	}
	printf("\n\n");
}

int compare2(Player *p, Player *q)   //p: people   q:computer
{
	int ret = p->type -q->type;
	if(ret == 6 || ret == -6)     //compare set and speciar cards 
		return -ret;
	if(ret != 0)
		return ret;
	switch (p->type){
		case SET:
		case STRAIGHT_FLUSH:
		case STRAIGHT:
			return p->handcards[2].point - q->handcards[2].point;
		case PAIR:
			return (p->handcards[0].point - q->handcards[0].point) != 0 ? p->handcards[0].point - q->handcards[0].point : p->handcards[2].point - q->handcards[2].point;
		case FLUSH:
		case SCATTERED:
			return (p->handcards[0].point - q->handcards[0].point) != 0 ?( p->handcards[0].point - q->handcards[0].point) :  \
				    (p->handcards[1].point - q->handcards[1].point) != 0 ? (p->handcards[1].point - q->handcards[1].point) : (p->handcards[2].point - q->handcards[2].point);
	}
	return 0;
		
}
void play_with_computer(Card *poker, Player *p)
{
	Player *winner = NULL;
	Player computer = {"computer"};
	int base, money;
	printf("Third step:\n\t:Plese set money of computer:\n");
	scanf("%d", &computer.balance);
	printf("Forth step:\n\t:Plese set money of you:\n");
	scanf("%d", &p->balance);
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
	init_poker(poker);
	do{
		printf("Handing...\n");
		sleep(1);
		shuffle_poker(poker);
		give_money(p, base);
		give_money(&computer, base);
		deal2(poker, p, &computer);
		sort(&computer.handcards);
		sort(&p->handcards);
		computer.type = judge_type(&computer.handcards);
		if(computer.type == SCATTERED)
			sort_scattered(&computer.handcards);
		p->type = judge_type(&p->handcards);
		if(p->type == SCATTERED)
			sort_scattered(&p->handcards);
		show_playercards(p);
		sleep(1);
		do{
			printf("Plesase input raise number or input 0 to give up\n");
			scanf("%d", &money);
			if(money < base && money != 0)
				printf("\nIput error,plese retry !");
		}while(money < base && money != 0);
		if (money != 0){
			give_money(p, money);
			give_money(&computer, money);
			if(compare2(p, &computer) > 0)
				winner = p;
			else
				winner = &computer;
		}
		else
			winner = &computer;
		show_playercards(&computer);
		printf("The winner is:\n\t %s\n\n", winner->name);
		get_money(winner,(money+base)*2);
		printf("Your money:    %d\tComputer's money:    %d\n\n",  p->balance, computer.balance);
		if((computer.balance <= 0) ||( p->balance <= 0)){
			if(computer.balance <= 0){
				printf("You win !!!\n");
				break;
			}
			else{
				printf("You lost  !!!\n");
				break;
			}
		}
		printf("Continue?  [Y/N]", p->balance, computer.balance);
		scanf(" %c", &c);
		printf("\n\n");
	}while(c=='Y'||c=='y');
	sleep(1);
	printf("\n\tGAME OVER!  THANKS\n");
	sleep(1);
	printf("\tDesigned by wlynn .\n");
	sleep(1);
	printf("\tThe game is for entertainment only, away fromt gambling !\n\n");
	sleep(2);
}




























