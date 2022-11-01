#ifndef _MAP_H
#define _MAP_H
#include "../../lib/menu.h"
// x,y 좌표로 커서 이동
#define gotoxy(x,y)	printf("[%d;%dH", y, x)
#define WIDTH 100
#define HEIGHT 23
#define ORIGINX 0
#define ORIGINY 1
typedef struct Map{
    struct Map* m_this;           
    char* frame;
    LPMENU lpMenu;

    void (*initMap)(struct Map *m_this);
    void (*showFrame)(struct Map *m_this);
    void (*showMenu)(struct Map *m_this);    
    void (*deleteMap)(struct Map *m_this);

    void (*clear)(struct Map *m_this);    
    void (*keyboardCallback)(struct Map *m_this);        
    void (*showPost)(struct Map *m_this);    
    void (*showSearch)(struct Map *m_this);    
    void (*showRevise)(struct Map *m_this);    
}Map;

Map *newMap();
void initMap_(Map *m_this);
void showFrame_(Map *m_this);
void showMenu_(Map *m_this);
void deleteMap_(Map *m_this);

void clear_(Map *m_this);
void keyboardCallback(Map *m_this);        
void showPost(Map *m_this);    
void showSearch(Map *m_this);  
void showRevise(Map *m_this);  

#endif
