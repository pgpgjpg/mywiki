#ifndef _MAP_H
#define _MAP_H

// x,y 좌표로 커서 이동
#define gotoxy(x,y)	printf("[%d;%dH", y, x)

typedef struct Map{
    struct Map *m_this; // 자신을 가리키는 포인터 변수
    
    char* m_map; // client에 전송할 map 저장
    int m_rows, m_cols, m_padding; // map의 해상도
    int m_stat; // 1.menu 2.search 3.upload 4.revise 5.exit

    void (*deleteMap)(struct Map *); // 소멸자
    void (*drawFrame)(struct Map *);
    void (*drawText)(struct Map *, int, int, char *);
    void (*clearMap)(struct Map *);
    void (*show)(struct Map *);
}Map;

Map *newMap();
void deleteMap_(Map *); // 소멸자
void drawFrame_(Map *);
void drawText_(Map *, int, int, char *);
void clearMap_(Map *);
void show_(Map *);

#endif
