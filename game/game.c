#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

const char* cardtype[ ] =  _HANDCARDS_TYPE;
const char* suitname[ ] = {"HEARTS", "SPADES", "DIAMONDS", "CLUBS"};
const char* pointname[ ] = { " ",  "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};


void init_poker(Card* poker)
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

