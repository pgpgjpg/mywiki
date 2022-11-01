#include <stdio.h>
#include "client.h"

int main()
{
    Client *client = newClient();
    client->setClient(client, "127.0.0.1");
    //client->recv(client);
    printf("Insert passwd : ");
    client->send(client);
    client->recv(client);
    client->deleteClient(client);
    return 0;
}