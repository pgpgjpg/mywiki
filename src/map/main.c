#include "map.h"
#include <stdio.h>

int main()
{
    Map *map = newMap();    
    map->drawFrame(map);
    map->drawText(map, map->m_rows/2, map->m_cols/2, "Test");
    map->show(map);
    map->deleteMap(map);
}