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

























#endif
