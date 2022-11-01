#include "map.h"

int main()
{
    Map *map = newMap();
    map->initMap(map);
    map->showFrame(map);
    //map->showMenu(map);    
    map->deleteMap(map);
}