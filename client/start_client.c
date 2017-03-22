#include "client.h"


int main(){
    int people_flag;
    char namebuf[20] = {};
    people_flag = init_game(namebuf);
    switch (people_flag){
        case 1:
            play_with_computer(namebuf);
            break;
        case 2:
            play_with_others(namebuf);
            break;
    }
    return 0;
}