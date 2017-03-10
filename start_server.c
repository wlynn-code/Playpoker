#include "server.h"
#include "game.h"

int main()
{
	signal(SIGINT, sig_close);
	init();
}
