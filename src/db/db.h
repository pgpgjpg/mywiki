#ifndef _DB_H
#define _DB_H
#include "array.h"
#include "profile.h"
#define DATA_COUNT "DATA_COUNT"
#define FILE_TITLE "FILE_TITLE"
#define FILE_DATA "FILE_DATA"
#define FILE_PATH "FILE_PATH"
#define FILE_TAGS "FILE_TAGS"

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
    void (*getTag)(struct DB *);
    int (*load)(struct DB *);
    int (*isInDB)(struct DB *, char*);
}DB;

DB *newDB();
void deleteDB_(DB *); // 소멸자
void save_(DB *, Data *);
void remove_(DB *, char *);
void getTag_(DB *);
int load_(DB *);
int isInDB_(DB *, char*);

#endif