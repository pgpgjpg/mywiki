#include <stdio.h>
#include "server.h"
#include <string.h>
#include <stdlib.h>

int main()
{    
    Server *server = newServer();
    server->set(server);    
    server->run(server);    
    server->deleteServer(server);
    return 0;
}