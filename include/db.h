#ifndef _DB_H
#define _DB_H
#include "array.h"
#include "profile.h"
#define DATA_COUNT "DATA_COUNT"
#define FILE_TITLE "FILE_TITLE"
#define FILE_DATA "FILE_DATA"
#define FILE_PATH "FILE_PATH"
#define FILE_TAGS "FILE_TAGS"
#define DB_PATH "/home/mobis/jpg/mywiki/data/server/db.txt"
#define EXAMPLE_PATH "/home/mobis/jpg/mywiki/data/server/example/"

typedef struct{
    char *title;
    char *data;
    char *file;
    LPARRAY tags;
} Data;
typedef struct DB{
    struct DB *m_this; // 자신을 가리키는 포인터 변수
    LPHASH m_lpHashTag;
    Data *m_lpData; // {char *title, char *data, char *file, LPARRAY tags;}    
    int m_nData;

    void (*deleteDB)(struct DB *); // 소멸자
    void (*save)(struct DB *, Data *);
    void (*remove)(struct DB *, char *);
    int (*load)(struct DB *);
    int (*isInDB)(struct DB *, char*);
}DB;

DB *newDB();
void deleteDB_(DB *); // 소멸자
void save_(DB *, Data *);
void remove_(DB *, char *);
int load_(DB *);
int isInDB_(DB *, char*);

#endif