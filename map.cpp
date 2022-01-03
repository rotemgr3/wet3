#include "map.h"

Map* MapCreate(void* keyArr, void* dataArr)
{
    Map *map = new Map();
    map->key = keyArr;
    map->data = dataArr;
    return map;
}

void MapDestroy(Map* map)
{
    delete map;
}