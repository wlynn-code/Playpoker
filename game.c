#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

const char* cardtype[ ] = _HANDCARDS_TYPE;
const char* suitname[ ] = {"HEARTS", "SPADES", "DIAMONDS", "CLUBS"};
const char* pointname[ ] = { " ",  'A', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K',}


void init_poker( Card poker[ ])
{
	int i = 0, suit = 0, point = 0;
	for(i = suit; suit < 4; suit++){
		for(point = 1; point <= 13; point++){
			poker[i].suit = suit;
			poker[i].point = point;
			i++;
		}
	}	
}

int init_game(Card poker[ ]){
	int people_num;
	char *people_name;
	init_poker(poker);
	printf("\nFirst step:\n\tPlese input how many people to play : \n");
	printf("\nHelp:\n");
	printf("\tinput 1: play with computer \n");
	printf("\tinput 2 or 3 or ... : play with others\n");
	scanf("%d", &people_num);
	printf("Next step:\n\tPlese input your name:\n");
	scanf("%s", &people_name);
	return people_num;
}

void swap(Card *p, Card *q)
{
	Card t = *p;
	*p = *q;
	*q = t;
}
void shuffle_poker(Card poker[ ])
{
	int i = 0;
	for(i = 0; i  < 200; i++){
		srand(time(NULL));
		swap( &poker[rand( )%52], &poker[rand( )%52]);
	}
}

void deal(Card poker[ ], Player *p, Player *q)
{
	int i = 0;
	for(i = 0; i < 3; i++){
		p -> handcards[i] = poker[ 2*i];
		q -> handcards[i] = poker[2*i+1]; 
	}
}

void sort(Card handcards[ ])
{
	if(handcards[0].point < handcards[1].point)
		swap(&handcards[0], &handcards[1]);
	if(handcards[0].point< handcards[2].point)
		swap(&handcards[0], &handcards[2]);
	if(handcards[1].point < handcards[2].point)
		swap(&handcards[1], &handcards[2]);
	if(handcards[2].point == 1 ) {
		if ((handcards[1].point  !=2) ||( handcards[0].point != 3)){
			swap(handcards[0],handcards[1]);
			swap(handcards[0],handcards[2]);
		}
	}
}

int  judge_type(Card handcards[ ])
{
	if((handcards[0].point =2) && (handcards[1].point = 3) && (handcards[2].point = 5))
		return SPECIAL;
	else if((handcards[0].point == handcards[1].point) ||( handcards[1] = handcards[2].point)){
		if(handcards[0].point == handcards[1].point == handcards[2].point)
			return SET;
		if handcards[1].point == handcards[2].point)
			swap(&handcards[0], &handcards[1])
		return PAIR;
	}
	else if((handcards[0].point+1) == (handcards[1].point) == (handcards[2].point -1)){
		if(handcards[0].suit == handcards[1].suit == handcards[2].suit)
			return STRAIGHT_FLUSH;
		else 
			return STRAIGHT;
	}
	else if(handcards[0].suit == handcards[1].suit == handcards[2].suit)
		return FLUSH;
	else
		return SCATTERED;
}





























