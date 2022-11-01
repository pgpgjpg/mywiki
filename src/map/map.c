#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "map.h"

Map *newMap()
{
    Map *tmp = (Map*)malloc(sizeof(Map));
    
    tmp->m_this = tmp;
    tmp->initMap = initMap_;
    tmp->showFrame = showFrame_;  
    tmp->showMenu = showMenu_;  
    tmp->deleteMap = deleteMap_;      

    return tmp;
}

void deleteMap_(Map *p_this)
{
    free(p_this);
}

void initMap_(Map *m_this)
{    
    int fd;
    struct stat fbuf;    

    // frame 초기화
    if((fd = open("/home/mobis/final_project/src/map/data/frame.txt", O_RDONLY)) == -1)
    {
        perror("open");
        exit(1);
    }

    fstat(fd, &fbuf);
    m_this->frame = (char*)malloc(sizeof(char)*fbuf.st_size);

    if(read(fd, m_this->frame, fbuf.st_size)==-1){
        perror("read");
        exit(1);
    }

    close(fd);     

    //MENU 메모리를 할당한다.    
    int nErr = menuCreate(&m_this->lpMenu, "/home/mobis/final_project/src/map/data/menu.txt");
    if (ERR_MENU_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);		
    }
}

void showFrame_(Map *m_this)
{
    printf("%s\n", m_this->frame);  
}

void showMenu_(Map *m_this)
{
    int nErr = menuRun(m_this->lpMenu);    
    if (ERR_MENU_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);		
    }
    
    gotoxy(0, 0);
}