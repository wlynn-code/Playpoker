#include "server.h"

int main()
{
    Card poker[52];
    signal(SIGINT, sig_close);
    init_server(poker);
    service(poker);
}
