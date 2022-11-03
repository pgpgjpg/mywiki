#include <stdio.h>
#include "db.h"

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
    db->deleteDB(db);    
    return 0;
}