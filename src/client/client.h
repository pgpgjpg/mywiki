#ifndef _CLIENT_H
#define _CLIENT_H
#include <arpa/inet.h>
#include <netdb.h>
#define MAXDATASIZE 3000
typedef struct Client{
    int sockfd;
    struct Client* m_this;
    struct hostent *he;
    struct sockaddr_in server_addr; 
    char buf[MAXDATASIZE];           
    
    void (*setClient)(struct Client *m_this, char* ipaddr);
    void (*recv)(struct Client *m_this);
    void (*send)(struct Client *m_this);
    void (*deleteClient)(struct Client *m_this);
    
}Client;

Client *newClient();
void deleteClient_(Client *server_ptr);
void setClient_(Client *p_this, char* ipaddr);
void send_(Client *p_this);
void recv_(Client *p_this);

#endif