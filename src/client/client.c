#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "client.h"

Client *newClient()
{
    Client *tmp = (Client*)malloc(sizeof(Client));
    
    tmp->m_this = tmp;
    tmp->setClient = setClient_;
    tmp->send = send_;    
    tmp->recv = recv_;  
    tmp->deleteClient = deleteClient_;      

    return tmp;
}

void deleteClient_(Client *p_this)
{
    close(p_this->sockfd);
    free(p_this);
}

void setClient_(Client *p_this, char* ipaddr)
{
   if((p_this->he = gethostbyname(ipaddr)) == NULL) {
       perror("gethostbyname");
       exit(1);
   }
   if((p_this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("socket");
       exit(1);
   }
   p_this->server_addr.sin_family = AF_INET;
   p_this->server_addr.sin_port = htons(60000);
   p_this->server_addr.sin_addr = *((struct in_addr *)p_this->he->h_addr);
   printf("[ %s ]\n",(char*) inet_ntoa(p_this->server_addr.sin_addr));
   memset(&(p_this->server_addr.sin_zero), '\0',8);
   if(connect(p_this->sockfd, (struct sockaddr *)&p_this->server_addr, sizeof(struct sockaddr))== -1) {
       perror("connect");
       exit(1);
   }
}

void send_(Client *p_this)
{
   while(1){
        fgets(p_this->buf, MAXDATASIZE, stdin);
        p_this->buf[strlen(p_this->buf)-1] = '\0';
        write(p_this->sockfd, p_this->buf, strlen(p_this->buf));   

        //system("clear");
        p_this->recv(p_this);       

        fflush(stdin);
   }
    
}

void recv_(Client *p_this)
{
    char recvbuf[MAXDATASIZE];   
    int numbytes;    
    fflush(stdout);
    if((numbytes = recv(p_this->sockfd, recvbuf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }    
    recvbuf[numbytes] = '\0';
    printf("%s\n", recvbuf);
    
}