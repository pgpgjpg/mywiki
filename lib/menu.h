#ifndef _MENU_H_DF294208_F203_4892_8059_44FA1D9BECEE_
#define _MENU_H_DF294208_F203_4892_8059_44FA1D9BECEE_

#include "profile.h"

#define MENU_MAGIC_CODE			0x4D454E55 /*MENU*/

#define ERR_MENU_OK				0
#define ERR_MENU_CREATE			-4000
#define ERR_MENU_MAGICCODE		-4001
#define ERR_MENU_ALLOC			-4002
#define ERR_MENU_OPEN		    -4003
#define ERR_MENU_NOT_FOUND		-4004
#define ERR_MENU_SHARED_OBJECT	-4005


#define MAIN_MENU_TITLE "MAIN_MENU_TITLE"
#define MAIN_MENU_TITLE_LOC "MAIN_MENU_TITLE_LOC"
#define MENU_COUNT      "MENU_COUNT"
#define MENU_TITLE_SO   "MENU_TITLE_SO"
#define MENU_TITLE_LOC  "MENU_TITLE_LOC"
#define MENU_TITLE      "MENU_TITLE"
#define MENU_COMMAND    "MENU_COMMAND"
#define CHOICE_MSG      "CHOICE_MSG"

// x,y 좌표로 커서 이동
#define gotoxy(x,y)	printf("[%d;%dH", y, x)

//함수 포인터 타입 선언 
typedef int (*LPFN_COMMAND)(void*);

//메뉴 제목과 메뉴 제목을 선택했을 때 동작할 함수 
typedef struct {
    char* sharedObjectName;  //공유라이브러리 이름 
    void* handle;            //공유라이브러리를 로드한 핸들 
    char* menuTitle;         //공유라이브러리에 있음 함수를 호출기 위한 화면 출력용 메시지 
    char* commandName;       //공유라이브러리에 있는 함수이름
    char* loc;
    LPFN_COMMAND fnCommand;  //공유라이브러리에 있는 함수 포인터 주소
} MENUITEM;

//메뉴를 관리할 구조체 
typedef struct {
    int       magicCode;
    LPPROFILE lpProfile;
    char*     mainTitle;
    char*     mainTitleLoc;    
    char*     choiceMsg;
    int       menuCount;    
    MENUITEM  menuItem[0];
} MENU;

typedef MENU *LPMENU;
typedef const MENU *LPC_MENU;

int menuCreate(LPMENU* lppRet, char* path);
int menuRun(LPC_MENU lpMenu);
int menuDestroy(LPMENU lpMenu);

#endif /*_MENU_H_DF294208_F203_4892_8059_44FA1D9BECEE_*/
