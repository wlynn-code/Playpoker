#include <stdio.h>
#include "game.h"
int main(){
	int pepole_num;
	Card  poker[52];
	Player plst_arr[5] = {};
	pepole_num = init_game(poker, plst_arr);
	switch (pepole_num){
		case 1:
			play_with_computer(poker, &plst_arr[0]);
		break;	
	}
	return 0;
}