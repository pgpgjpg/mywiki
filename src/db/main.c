#include <stdio.h>
#include "db.h"
#include "../finder/finder.h"

int main()
{    
    DB *db = newDB();
    db->load(db);    

    Data dt;
    dt.title = "title";
    dt.data = "data";
    dt.file = "file";
    arrayCreate(&dt.tags);
    arrayAdd(dt.tags, "a");
    arrayAdd(dt.tags, "b");
    db->save(db, &dt);



    Finder *finder = newFinder();
    finder->initFinder(finder);
    if(!finder->loadDataByTitle(finder, db->m_lpData, db->m_nData, "main")){
        printf("error : finder->loadDataByTitle\n");
        return 0;
    }
    finder->showData(finder);
    finder->searchTitleByTag(finder, db->m_lpHashTag, "a");  
    printf("입력한 tag : %s\n", "a");  
    finder->showTitles(finder);

    db->deleteDB(db);    
    finder->deleteFinder(finder);
    return 0;
}