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

    tmp->deleteMap = deleteMap_;
    tmp->drawFrame = drawFrame_;
    tmp->drawText = drawText_;
    tmp->clearMap = clearMap_;
    tmp->keyboardCallback = keyboardCallback_;
    tmp->show = show_;
    tmp->m_this = tmp;       
    
    return tmp;
}

void deleteMap_(Map *lpMap)
{
    free(lpMap);
}

void drawFrame_(Map *lpMap)
{
    int fd;
    struct stat fbuf;    

    FILE *f;
    f = fopen("/home/mobis/jpg/mywiki/src/map/data/frame.txt", "r");
    fseek(f, 0, SEEK_END);    
        
    // frame 초기화
    if((fd = open("/home/mobis/jpg/mywiki/src/map/data/frame.txt", O_RDONLY)) == -1)
    {
        perror("open");
        exit(1);
    }
    
    lpMap->m_map = (char*)malloc(sizeof(char)*ftell(f));
    
    // fstat(fd, &fbuf);
    // lpMap->m_map = (char*)malloc(sizeof(char)*fbuf.st_size);

    if(read(fd, lpMap->m_map, ftell(f))==-1){
        perror("read");
        exit(1);
    }    

    fclose(f);
    close(fd);     

    lpMap->m_rows = lpMap->m_cols = lpMap->m_padding = 0;
    for(int i = 0; i < fbuf.st_size; ++i){
        lpMap->m_padding++;
        if(lpMap->m_map[i] == '\n') break;
    }
    for(int i = 0; i < fbuf.st_size; ++i){        
        if(lpMap->m_map[i] == '\n'){            
            lpMap->m_rows++;            
            lpMap->m_cols = 0;            
        }
        lpMap->m_cols++;
    }
    lpMap->m_rows++;    
    lpMap->m_cols /= 3;
    lpMap->m_cols += 5; // 양 끝 테두리에 있는 2개의 '│'이 각각 3byte라서 +4 덧셈 and 개행문자 +1    
}


void drawText_(Map *lpMap, int r, int c, char *text)
{
    int idx = lpMap->m_padding + r*lpMap->m_cols + c;
    
    memmove(lpMap->m_map + idx, text, strlen(text));
}

void clearMap_(Map *lpMap)
{    
    lpMap->drawFrame(lpMap);
}

void keyboardCallback_(Map *lpMap)
{

}

void show_(Map *lpMap)
{
    printf("%s\n", lpMap->m_map);
}