#ifndef GAME_H
#define GAME_H
typedef unsigned int  uint;


enum Suit {
	hearts,
	spades,
	diamonds,
	clubs
};

typedef struct{
	uint suit:2;
	uint point:4;
}Card;

typedef struct{
	char name[20];
	Card handcards[3];
	int type;
	int balance;
	int fds;
         int iscard;
}Player;

enum Type{
	SPECIAL = 1,
	SCATTERED,
	PAIR,
	STRAIGHT,
	FLUSH,
	STRAIGHT_FLUSH,
	SET
};

#define _HANDCARDS_TYPE  { \
	"SPECIAL", \
	"SCATTERED", \
	"PAIR", \
	"STRAIGHT", \
	"FLUSH",  \
	"STRAIGHT FLUSH", \
	"SET"  \
	}
extern const char* cardtype[ ];
extern const char* suitname[ ];
extern const char* pointname[ ];
extern void init_poker(Card* poker);
extern void swap(Card *p, Card *q);
extern void shuffle_poker(Card *poker);
extern void give_money(Player *p, int money);
extern void get_money(Player *p, int money);
extern void deal2(Card *poker, Player *p, Player *q);
extern void sort(Card *handcards);
extern void sort_scattered(Card *handcards);
extern int  judge_type(Card *handcards);
extern void show_playercards(Player *p);
extern int compare2(Player *p, Player *q);














#endif
