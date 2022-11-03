#include <stdio.h>
#include "db.h"

int main()
{    
    DB *db = newDB();
    db->load(db);    
    db->deleteDB(db);    
    return 0;
}