#include <stdio.h>
#include "server.h"
#include <string.h>
#include <stdlib.h>

int main()
{    
    Server *server = newServer();
    server->setServer(server);    
    server->recv(server);    
    server->deleteServer(server);
    return 0;
}