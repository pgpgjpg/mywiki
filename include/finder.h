#ifndef _FINDER_H
#define _FINDER_H
#include "array.h"
#include "hash.h"
#include "db.h"

typedef struct Finder{
    struct Finder *m_this; // 자신을 가리키는 포인터 변수
    Data m_data; // {char *title, char *data, char *file, LPARRAY tag;}
    LPHASH m_lpHashTitle;
    
    int (*initFinder)(struct Finder *); 
    void (*deleteFinder)(struct Finder *); // 소멸자
    int (*searchTitleByTag)(struct Finder *, LPHASH, char *);
    int (*loadDataByTitle)(struct Finder *, Data *, int nData, char*);
    void (*showData)(struct Finder *);
    void (*showTitles)(struct Finder *);
}Finder;

Finder *newFinder();
int initFinder_(Finder *); 
void deleteFinder_(Finder *); // 소멸자
int searchTitleByTag_(Finder *, LPHASH, char *);
int loadDataByTitle_(Finder *, Data *,int, char*);
void showData_(Finder *);
void showTitles_(Finder *);

#endif